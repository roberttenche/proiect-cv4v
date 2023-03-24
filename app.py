from flask import Flask, Response, redirect, url_for
import cv2 as cv
import threading

import main

app = Flask(__name__)

last_frame = None

@app.route('/')
def index():
    return redirect(url_for('live_feed'), code=302)

@app.route('/live_feed')
def live_feed():
    return Response(
        get_frame(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

def get_frame():
    global last_frame
    while True:
        if len(main.image_buffer) <= 0:
            yield (
                b'--frame\r\n'
                b'Content-Type: image/jpg\r\n\r\n' +
                last_frame +
                b'\r\n\r\n'
            )

        ret, jpg = cv.imencode('.jpg', main.image_buffer.pop())

        frame = jpg.tobytes()

        last_frame = frame

        yield(
            b'--frame\r\n'
            b'Content-Type: image/jpg\r\n\r\n' +
            frame +
            b'\r\n\r\n'
        )

if __name__ == '__main__':
    video_file = 'rec/test2.mp4'

    main_thread  = threading.Thread(target=main.start, args=[video_file])
    main_thread.start()
    app.run(host='0.0.0.0', port=5000, debug=False)
