import os
import cv2
import numpy as np
import matplotlib.pyplot as plt

# --------------------------
# Utility / preprocessing
# --------------------------
def ensure_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

def to_gray(img):
    if len(img.shape) == 3:
        return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    return img

def normalize_uint8(img):
    # take float array -> scale to 0..255 and uint8
    if img.dtype == np.uint8:
        return img
    img = np.asarray(img, dtype=np.float32)
    mn, mx = img.min(), img.max()
    if mx - mn == 0:
        return np.zeros_like(img, dtype=np.uint8)
    img = (img - mn) / (mx - mn) * 255.0
    return img.astype(np.uint8)

# --------------------------
# Noise functions
# --------------------------
def add_salt_pepper(img, amount=0.02):
    """
    amount: fraction of pixels to alter (e.g. 0.02 = 2%)
    """
    out = img.copy()
    h, w = img.shape
    num = int(amount * h * w)
    # salt
    coords = (np.random.randint(0, h, num), np.random.randint(0, w, num))
    out[coords] = 255
    # pepper
    coords = (np.random.randint(0, h, num), np.random.randint(0, w, num))
    out[coords] = 0
    return out

def add_gaussian_noise(img, mean=0, sigma=15):
    gauss = np.random.normal(mean, sigma, img.shape).reshape(img.shape)
    noisy = img.astype(np.float32) + gauss
    return normalize_uint8(noisy)

# --------------------------
# Restoration functions
# --------------------------
def median_restore(img, ksize=3):
    return cv2.medianBlur(img, ksize)

def gaussian_restore(img, ksize=3, sigma=0):
    return cv2.GaussianBlur(img, (ksize, ksize), sigma)

# --------------------------
# Edge operators
# --------------------------
def roberts_edge(img):
    # Roberts kernels (2x2)
    kx = np.array([[1, 0],
                   [0,-1]], dtype=np.float32)
    ky = np.array([[0, 1],
                   [-1,0]], dtype=np.float32)
    gx = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, kx)
    gy = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, ky)
    mag = np.sqrt(gx*gx + gy*gy)
    return normalize_uint8(mag)

def prewitt_edge(img):
    px = np.array([[-1, 0, 1],
                   [-1, 0, 1],
                   [-1, 0, 1]], dtype=np.float32)
    py = np.array([[ 1,  1,  1],
                   [ 0,  0,  0],
                   [-1, -1, -1]], dtype=np.float32)
    gx = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, px)
    gy = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, py)
    mag = np.sqrt(gx*gx + gy*gy)
    return normalize_uint8(mag)

def sobel_edge(img):
    gx = cv2.Sobel(img.astype(np.float32), cv2.CV_32F, 1, 0, ksize=3)
    gy = cv2.Sobel(img.astype(np.float32), cv2.CV_32F, 0, 1, ksize=3)
    mag = np.sqrt(gx*gx + gy*gy)
    return normalize_uint8(mag)

def freienchen_edge(img):
    f = np.sqrt(2.0)
    fx = np.array([[ 1,  0, -1],
                   [ f,  0, -f],
                   [ 1,  0, -1]], dtype=np.float32)
    fy = np.array([[ 1,  f,  1],
                   [ 0,  0,  0],
                   [-1, -f, -1]], dtype=np.float32)
    gx = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, fx)
    gy = cv2.filter2D(img.astype(np.float32), cv2.CV_32F, fy)
    mag = np.sqrt(gx*gx + gy*gy)
    return normalize_uint8(mag)

# dictionary for convenience
EDGE_FUNCS = {
    "Roberts": roberts_edge,
    "Prewitt": prewitt_edge,
    "Sobel": sobel_edge,
    "FreiChen": freienchen_edge
}

# --------------------------
# Main pipeline
# --------------------------
def main_pipeline(input_path='input.png', save_outputs=True, show_plots=True):
    ensure_dir('outputs')
    # load or synthesize
    if os.path.exists(input_path):
        img_color = cv2.imread(input_path, cv2.IMREAD_UNCHANGED)
        gray = to_gray(img_color)
        gray = normalize_uint8(gray)
    else:
        print("input.png not found — generating a sample synthetic image (256x256).")
        # synthetic test image: shapes
        gray = np.zeros((256,256), dtype=np.uint8)
        cv2.rectangle(gray, (30,30), (220,220), 150, -1)
        cv2.circle(gray, (128,128), 60, 255, -1)
        cv2.line(gray, (0,255),(255,0), 200, 3)
        cv2.putText(gray, 'TEST', (60,140), cv2.FONT_HERSHEY_SIMPLEX, 1.2, 30, 2, cv2.LINE_AA)
        gray = normalize_uint8(gray)

    # save base grayscale
    if save_outputs:
        cv2.imwrite('gray.png', gray)

    # create noisy versions
    sp = add_salt_pepper(gray, amount=0.03)
    gauss = add_gaussian_noise(gray, mean=0, sigma=12)
    # restored image (median on salt-pepper)
    restored = median_restore(sp, ksize=3)

    if save_outputs:
        cv2.imwrite('salt_pepper.png', sp)
        cv2.imwrite('gaussian.png', gauss)
        cv2.imwrite('restored.png', restored)

    images = {
        "Grayscale": gray,
        "SaltPepper": sp,
        "Gaussian": gauss,
        "Restored": restored
    }

    # apply all edge funcs and save
    results = {}
    for img_name, img in images.items():
        results[img_name] = {}
        for method_name, func in EDGE_FUNCS.items():
            out = func(img)
            results[img_name][method_name] = out
            if save_outputs:
                fn = f"outputs/{method_name}_{img_name}.png"
                cv2.imwrite(fn, out)

    # show visual summary (for each image, show 4 methods)
    if show_plots:
        for img_name in results:
            fig, axes = plt.subplots(1, 5, figsize=(16,4))
            plt.suptitle(f"Edge Results - {img_name}", fontsize=14)
            axes[0].imshow(images[img_name], cmap='gray')
            axes[0].set_title('Original')
            axes[0].axis('off')
            idx = 1
            for method in results[img_name]:
                axes[idx].imshow(results[img_name][method], cmap='gray')
                axes[idx].set_title(method)
                axes[idx].axis('off')
                idx += 1
            plt.tight_layout()
            plt.show()

    print("All done. Outputs saved to ./ (gray.png, salt_pepper.png, gaussian.png, restored.png) and ./outputs/*.png")
    return images, results

if __name__ == "__main__":
    main_pipeline(input_path='ori.png')
