import cv2 as cv

image_buffer = []
is_running = False

def start(video_path : str):

    vidcap = cv.VideoCapture(video_path)
    count = 0

    success ,image = vidcap.read()
    image = apply_processing(image)
    image_buffer.insert(0, image)

    while success:
        success, image = vidcap.read()
        image = apply_processing(image)
        image_buffer.insert(0, image)
        count += 1

def apply_processing(image):
    # image = cv.cvtColor(image, cv.COLOR_RGB2GRAY)
    blur = cv.GaussianBlur(image,(5,5), 0)
    canny = cv.Canny(blur, 0, 255)
    return canny

if __name__ == '__main__':
    start('rec/test1.mp4')