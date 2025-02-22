//Main.cpp

/*
TODO:
Input verification of course code
Make functions and cpp/h files
Different way to load haar cascades(?)
Find a way to use haarcascade_eye_tree_eyeglasses.xml since reflection from glasses prevent facial detection
Detect face when tilted
*/

#include "main.h"

vector<string> training_filenames;
void load_training_directory() {
	DIR *pdir = NULL;
	pdir = opendir("./training_data");

	struct dirent * pent = NULL;

	if (pdir == NULL) {
		cerr << "couldn't load training data." << endl;
		exit(-1);
	}
	training_filenames.clear();
	while (pent = readdir(pdir)) {
		if (pent == NULL) {
			cerr << "pent couldn't be initialized properly" << endl;
			exit(-1);
		}
		if (pent->d_name[0] == '.') 
			continue;
		
		cout << "adding " << pent->d_name << " to the list of filenames for training" << endl;
		training_filenames.push_back(pent->d_name);
	}

}

//Haar cascades
CascadeClassifier faceCascade;	//Haar cascade classifier for face
CascadeClassifier eyeCascade;	//Haar cascade classifier for eyes
CascadeClassifier profileCascade;	//Haar cascade classfier for profile face

VideoCapture capture(0); //0 for default camera

string courseCode;	//Name of course

int main(int argc, char** argv) {
	load_training_directory();

	if (!faceCascade.load(FACE_CASCADE_LOCATION)) {
		cerr << "ERROR: cannot load " << FACE_CASCADE_LOCATION;
		exit(100);
	}
	if (!eyeCascade.load(EYE_CASCADE_LOCATION)) {
		cerr << "ERROR: cannot load " << EYE_CASCADE_LOCATION;
		exit(101);
	}
	if (!profileCascade.load(PROFILE_FACE_CASCADE_LOCATION)) {
		cerr << "ERROR: CAN'T LOAD" << PROFILE_FACE_CASCADE_LOCATION;
		exit(102);
	}

	//---Webcam---//
	if (!capture.isOpened()) {
		cerr << "ERROR: Cannot initialise camera";
		exit(200);
	}
	capture.set(CV_CAP_PROP_BUFFERSIZE, 1); //Set how many frames the buffer will store

	//---Course---//	
	if (PRINTCSV) {
		cout << "What is the course code? ";
		cin >> courseCode;
		transform(courseCode.begin(), courseCode.end(), courseCode.begin(), ::tolower);	//Make course code lowercase
	}

	//---Timer---//
	clock_t  timerPrev = clock();	//Time of the last frame shot
	double timerDuration = 0;	//Time difference between current time and time of last frame shot

	//---Time information---//
	time_t start = time(0);	//Get time of start of lecture
	time_t now = time(0);	//Get current time
	struct tm* tmStart = localtime(&start);	//Convert current time into data structure
	struct tm* tmNow = localtime(&now);	//Create time structure for output of results
	string tString = "";	//Date format (24 characters): yyyy-mm-dd-hh-mm-ss
	ostringstream tTemp;	//Temporary string to store time information
	int timeElapsed = 0;	//Time since beginning of lecture in seconds
	int timeStart = (tmStart->tm_hour) * 60 + (tmStart->tm_min) * 60 + (tmStart->tm_sec);	//Time of start of lecture in seconds
	int timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);	//Time now in seconds

	tTemp << tmStart->tm_year + 1900;	//Get year since 1990
	tString += tTemp.str() + "-";
	tTemp.str(string());	//Clear stringstream
	tTemp << tmStart->tm_mon + 1;	//Get number of months since January
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_mday;	//Get day of the month
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_hour;	//Get hour since midnight
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_min;	//Get minute
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_sec;	//Get seconds
	tString += tTemp.str();
	tTemp.str(string());

	//---Output file---//
	ofstream ofs;
	if (PRINTCSV) {
		string file = tString + '-' + courseCode + ".csv";
		ofs.open(file);	//Output file stream, data format: seconds since start of lecture, results\n
		ofs << tString << '-' << courseCode << '\n';
	}

	//---Frame information---//
	Mat frame;	//Captured frame
	Mat frameGray;	//To preprocess the captured frame
	Mat frameNormalizedFaces; //For the normalized faces

	vector<Rect> faces;	//Contain output of face detector
	vector<Rect> eyes;	//Contain output of eye detector
	vector<Rect> profiles;

	FixedQueue q_plot = FixedQueue(MAX_Q_SIZE);
	int arr_plot[MAX_ARR_SIZE];
	int arr_index = 0;
	int max_x = 1;
	int max_y = 1;

	//testing the graph stuff
	overviewWindow::getInstance().initialize(CANVAS_WIDTH, CANVAS_HEIGHT);
	
	//Start the window thread
	startWindowThread();
	namedWindow("windowNormalizedFaces");

	while (true) {
		if (waitKey(1) == ESC) //Press esc to exit
			break;

		timerDuration = (clock() - timerPrev) / (double)CLOCKS_PER_SEC;

		capture >> frame;	//Capture frame
		cvtColor(frame, frameGray, CV_BGR2GRAY);	//Convert frame from BGR to gray for easier processing
		equalizeHist(frameGray, frameGray);	//Normalize brightness and increase contrast: src,dest
		faceCascade.detectMultiScale(
			frameGray,
			faces,
			1.2,
			10,
			0 | CV_HAAR_SCALE_IMAGE,
			Size(0, 0),
			Size(300, 300)
			); //Detect faces, can also try CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING

		for (int i = 0; i < faces.size(); i++) {
			//---Detect size of each face---//
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
			Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1

				//---Draw rectangles around each face---//
				rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
			}

		imshow("Camera", frame);	//Output the processed image

		if (timerDuration > TIMER_INTERVAL) {
			/*
			overviewWindow::getInstance().clear();
			q_plot.enqueue(Point(0, static_cast<int>(faces.size() * 50)));
			for (int i = 0; i < MAX_Q_SIZE; i++) {
				overviewWindow::getInstance().addCircle(q_plot.get(i).x, CANVAS_HEIGHT - q_plot.get(i).y, 5, 1, 1);
			}
			q_plot.incrementXval(10);

			if (DEBUG) {
				cout << timeElapsed << "\tNumber of faces: " << faces.size() << endl;
			}
			*/

			if (arr_index < MAX_ARR_SIZE) {
				overviewWindow::getInstance().clear();
				overviewWindow::getInstance().addLine(0, CANVAS_HEIGHT - 50, CANVAS_WIDTH, CANVAS_HEIGHT - 50); //x-axis
				overviewWindow::getInstance().addLine(50, 0, 50, CANVAS_HEIGHT); //y-axis
				overviewWindow::getInstance().addText("0", 15, CANVAS_HEIGHT - 15);

				arr_plot[arr_index] = static_cast<int>(faces.size());

				if (max_y < arr_plot[arr_index]) {
					max_y = arr_plot[arr_index];
				}

				for (int i = 0; i < arr_index; i++) {
					overviewWindow::getInstance().addCircle(60 + i * 10,(int)(0.9 * (27 + CANVAS_HEIGHT - (double)arr_plot[i] / (double)max_y * CANVAS_HEIGHT)), 5, 1, 1);
					if (i != 0) {
						overviewWindow::getInstance().addLine(60 + (i - 1) * 10, (int)(0.9 * (27 + CANVAS_HEIGHT - (double)arr_plot[i - 1] / (double)max_y * CANVAS_HEIGHT)),
																60 + i * 10, (int)(0.9 * (27 + CANVAS_HEIGHT - (double)arr_plot[i] / (double)max_y * CANVAS_HEIGHT)));
					}
				}

				if (arr_index != 0) {
					//overviewWindow::getInstance().addLine((arr_index - 1) * 50, arr_plot[arr_index - 1], arr_index * 50, arr_plot[arr_index]);
				}
				arr_index++;
			}

			/* //detect profiles (when face is turned to side)
			profileCascade.detectMultiScale(
				frameGray,
				profiles,
				1.1,
				10,
				0 | CV_HAAR_SCALE_IMAGE,
				cvSize(0, 0),
				cvSize(300, 300)
			);
			for (int k = 0; k < profiles.size(); k++) {
				Point pt1(profiles[k].x + profiles[k].width, profiles[k].y + profiles[k].height);
				Point pt2(profiles[k].x, profiles[k].y);
				Mat profileROI = frameGray(profiles[k]);
				rectangle(frame, pt1, pt2, cvScalar(255, 0, ), 2, 8, 0);
			}*/

			//---Loop through each face---//
			int numFaces = static_cast<int>(faces.size());
			Mat frameAllNormalizedFaces = Mat::zeros(FACE_SIZE, FACE_SIZE * MAX_FACES_PER_ROW, CV_8UC3);
			Mat frameRowNormalizedFaces = Mat::zeros(FACE_SIZE, FACE_SIZE * MAX_FACES_PER_ROW, CV_8UC3);
			int i = 0; //Index to iterate through 

			for (int row = 0; row < numFaces / MAX_FACES_PER_ROW; row++) {
				for (int col = 0; col < MAX_FACES_PER_ROW; col++) {
					Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
					Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1
					Mat faceROI = frameGray(faces[i]);	//Stores current face of the loop

					resize(faceROI, faceROI, Size(FACE_SIZE, FACE_SIZE)); //Scale face into square shape
					if (col == 0) { //On new iteration of detection
						frameRowNormalizedFaces = Mat::zeros(FACE_SIZE, FACE_SIZE, CV_8UC3); //Set the faces frame to black
						frameRowNormalizedFaces = faceROI.clone(); //Clone the first face into the frame
					} else { //If we have >1 face
						hconcat(frameRowNormalizedFaces, faceROI, frameRowNormalizedFaces); //Horizontally concatenate the face into frame
					}
					i++;
				}
				if (row == 0) {
					frameAllNormalizedFaces = frameRowNormalizedFaces.clone();
				} else {
					vconcat(frameAllNormalizedFaces, frameRowNormalizedFaces, frameAllNormalizedFaces);
				}
			}

			//But what if we don't have perfect multiple of MAX_FACES_PER_ROW?
			if (numFaces % MAX_FACES_PER_ROW != 0) {
				int k;
				for (k = 0; k < numFaces % MAX_FACES_PER_ROW; k++) {
					Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
					Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1
					Mat faceROI = frameGray(faces[i]);	//Stores current face of the loop

					resize(faceROI, faceROI, Size(FACE_SIZE, FACE_SIZE)); //Scale face into square shape
					if (k == 0) { //On new iteration of detection
						//frameRowNormalizedFaces = Mat::zeros(FACE_SIZE, FACE_SIZE, CV_8UC3); //Set the faces frame to black
						frameRowNormalizedFaces = faceROI.clone(); //Clone the first face into the frame
					} else { //If we have >1 face
						hconcat(frameRowNormalizedFaces, faceROI, frameRowNormalizedFaces); //Horizontally concatenate the face into frame
					}
					i++;
				}
				Mat blank = Mat::zeros(FACE_SIZE, FACE_SIZE, CV_8UC3);
				cv::cvtColor(blank, blank, CV_RGB2GRAY); //Can't merge grey matrix with colored one, just convert blank to grey
				for (; k < MAX_FACES_PER_ROW; k++) {
					hconcat(frameRowNormalizedFaces, blank, frameRowNormalizedFaces); //Fill the rest of the row with blanks
				}
				if (numFaces < MAX_FACES_PER_ROW) {
					frameAllNormalizedFaces = frameRowNormalizedFaces.clone();
				} else {
					vconcat(frameAllNormalizedFaces, frameRowNormalizedFaces, frameAllNormalizedFaces);
				}
			}

			for (int i = 0; i < faces.size(); i++) {

				//---Detect size of each face---//
				Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
				Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1
				Mat faceROI = frameGray(faces[i]);	//Stores current face of the loop

			   /*
			   //---Detect eyes---//
			   eyeCascade.detectMultiScale(
				   faceROI,
				   eyes,
				   1.1,
				   2,
				   0 | CV_HAAR_SCALE_IMAGE,
					Size(30, 30)
			   );
			   for (size_t j = 0; j < eyes.size(); j++)
			   {
					Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);	//Find the center of each eye
				   int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);	//Find the radius of each eye
				   circle(frame, center, radius,  Scalar(255, 0, 0), 2, 8, 0);	//Draw a circle around each eye
			   }*/
			   /*
			   resize(faceROI, faceROI, Size(FACE_SIZE, FACE_SIZE)); //scale face into square shape
			   if (i == 0) { //on new iteration of detection
				   frameNormalizedFaces = Mat::zeros(FACE_SIZE, FACE_SIZE, CV_8UC3); //set the faces frame to black
				   frameNormalizedFaces = faceROI.clone(); //clone the first face into the frame
			   }
			   else { //if we have >1 face
				   hconcat(frameNormalizedFaces, faceROI, frameNormalizedFaces); //horizontally concatenate the face into frame
			   }
			   */
			}

			imshow("windowNormalizedFaces", frameAllNormalizedFaces);

			timeElapsed = timeNow - timeStart;

			if (PRINTCSV) {
				ofs << timeElapsed << ',' << faces.size() << '\n';
			}

			now = time(0);
			tmNow = localtime(&now);
			timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);
			timerPrev = clock(); //Reset comparison time for timer
		}
	}

	if (PRINTCSV) {
		ofs.close();	//Close output file stream
	}
	//No need to deintialise webcam

	return 0;
}