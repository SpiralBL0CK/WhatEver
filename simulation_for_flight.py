import numpy as np
import imutils
import cv2
 
import os
import random
import time

from imutils import paths
from imutils.video import VideoStream
from imutils.video import FPS

def real_time_detection():
    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--prototxt", required=True,
        help="path to Caffe 'deploy' prototxt file")
    ap.add_argument("-m", "--model", required=True,
        help="path to Caffe pre-trained model")
    ap.add_argument("-c", "--confidence", type=float, default=0.2,
        help="minimum probability to filter weak detections")
    args = vars(ap.parse_args())
    CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor"]
    COLORS = np.random.uniform(0, 255, size=(len(CLASSES), 3))

    # load our serialized model from disk
    print("[INFO] loading model...")
    net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])

    # initialize the video stream, allow the cammera sensor to warmup,
    # and initialize the FPS counter
    print("[INFO] starting video stream...")
    vs = VideoStream(src=0).start()
    time.sleep(2.0)
    fps = FPS().start()

    # loop over the frames from the video stream
    while True:
        # grab the frame from the threaded video stream and resize it
        # to have a maximum width of 400 pixels
        frame = vs.read()
        frame = imutils.resize(frame, width=400)

        # grab the frame dimensions and convert it to a blob
        (h, w) = frame.shape[:2]
        blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)),
            0.007843, (300, 300), 127.5)

        # pass the blob through the network and obtain the detections and
        # predictions
        net.setInput(blob)
        detections = net.forward()

        # loop over the detections
        for i in np.arange(0, detections.shape[2]):
            # extract the confidence (i.e., probability) associated with
            # the prediction
            confidence = detections[0, 0, i, 2]

            # filter out weak detections by ensuring the `confidence` is
            # greater than the minimum confidence
            if confidence > args["confidence"]:
                # extract the index of the class label from the
                # `detections`, then compute the (x, y)-coordinates of
                # the bounding box for the object
                idx = int(detections[0, 0, i, 1])
                box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
                (startX, startY, endX, endY) = box.astype("int")

                # draw the prediction on the frame
                label = "{}: {:.2f}%".format(CLASSES[idx],
                    confidence * 100)
                cv2.rectangle(frame, (startX, startY), (endX, endY),
                    COLORS[idx], 2)
                y = startY - 15 if startY - 15 > 15 else startY + 15
                cv2.putText(frame, label, (startX, y),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, COLORS[idx], 2)

        # show the output frame
        cv2.imshow("Frame", frame)
        key = cv2.waitKey(1) & 0xFF

        # if the `q` key was pressed, break from the loop
        if key == ord("q"):
            break

        # update the FPS counter
        fps.update()

    # stop the timer and display FPS information
    fps.stop()
    print("[INFO] elapsed time: {:.2f}".format(fps.elapsed()))
    print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))

    # do a bit of cleanup
    cv2.destroyAllWindows()
    vs.stop()

    def find_marker(image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (5, 5), 0)
        edged = cv2.Canny(gray, 35, 125)

        # find the contours in the edged image and keep the largest one;
        # we'll assume that this is our piece of paper in the image
        cnts = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        c = max(cnts, key = cv2.contourArea)

            # compute the bounding box of the of the paper region and return it
        return cv2.minAreaRect(c)

    def distance_to_camera(knownWidth, focalLength, perWidth):
        # compute and return the distance from the maker to the camera
        return (knownWidth * focalLength) / perWidth
        
    # initialize the known distance from the camera to the object, which
    # in this case is 24 inches
    KNOWN_DISTANCE = 24.0
    
    # initialize the known object width, which in this case, the piece of
    # paper is 12 inches wide
    KNOWN_WIDTH = 11.0
    
    # load the furst image that contains an object that is KNOWN TO BE 2 feet
    # from our camera, then find the paper marker in the image, and initialize
    # the focal length
    image = cv2.imread("images/2ft.png")
    marker = find_marker(image)
    focalLength = (marker[1][0] * KNOWN_DISTANCE) / KNOWN_WIDTH

    for imagePath in sorted(paths.list_images("images")):
        # load the image, find the marker in the image, then compute the
        # distance to the marker from the camera
        image = cv2.imread(imagePath)
        marker = find_marker(image)
        inches = distance_to_camera(KNOWN_WIDTH, focalLength, marker[1][0])
    
        # draw a bounding box around the image and display it
        box = cv2.cv.BoxPoints(marker) if imutils.is_cv2() else cv2.boxPoints(marker)
        box = np.int0(box)
        cv2.drawContours(image, [box], -1, (0, 255, 0), 2)
        cv2.putText(image, "%.2fft" % (inches / 12),
            (image.shape[1] - 200, image.shape[0] - 20), cv2.FONT_HERSHEY_SIMPLEX,
            2.0, (0, 255, 0), 3)
        cv2.imshow("image", image)
        cv2.waitKey(0)

def pause():
    programPause = raw_input("Press the <ENTER> key to continue...")
     
def simulate_position(size):
    x =  np.arange(2, 11).reshape(3,3)
    #print x
    return x

def generate_obstacles():
    obstacles = [[[random.randint(0,1) for x in xrange(3)] for y in xrange(3)] for z in range(3)]
    random.shuffle(obstacles)
    return obstacles

def format_position(xs,ys,zs,simulated_position):
    for i in range(3):
        xs.append(simulated_position[i][0])
        ys.append(simulated_position[i][1])
        zs.append(simulated_position[i][2])
    return (xs,ys,zs)





def main():
    xs = []
    ys = []
    zs = []
    status = 1
    steps = 0
    i=0
    counter = 0
    pasi = 0

    obstacles = generate_obstacles()
    format_position(xs,ys,zs,obstacles)
    positions = simulate_position(3)
    if obstacles[0][0][2] == 0:
        print "We are good to fly"
    else:
        print "we cant fly"
        quit(-1)

    #print obstacles
    for i in range(3):
        for j in range(3):
            for k in range(3):
                if obstacles[i][j][k] == 1:
                        if k == 0:
                            if obstacles[i][j][k+1] == 0:
                                print "we are good to move on y asis"
                                print("Position of matrix[{}][{}][{}]".format(i,j,k))
                            else:
                                if obstacles[i][j][k+2] == 0:
                                    print "we are good to move on z asis"
                                    print("Position of matrix[{}][{}][{}]".format(i,j,k))
                        if k == 1:
                            if obstacles[i][j][k-1] == 0:
                                print "we are good to move on x asis"
                                print("Position of matrix[{}][{}][{}]".format(i,j,k))
                            else:
                                if obstacles[i][j][k-1] == 0:
                                    print "we are good to move on x asis"
                                    print("Position of matrix[{}][{}][{}]".format(i,j,k))

                        print("Position of obstacle: matrix[{}][{}][{}]".format(i,j,k))
                        print i,j,k
                        print obstacles

if __name__ == '__main__':
    main()





"""
cells = [[[x*y*z for z in range(3)] for y in range(3)] for x in range(3)]
for x,y,z in itertools.product(range(3), range(3), range(3)):
    print("(%d, %d, %d) %d" % (x,y,z,cells[x][y][z]))
"""