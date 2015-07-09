#include "facedetector2.h"

using namespace std;
using namespace cv;

Mat faceDetector2::crop;
string faceDetector2::box_text;

static void read_csv(const string& filenamex, vector<Mat>& images, vector<int>& labels, char separator = ';') {

    std::cout<<"filename :: "<<filenamex<<endl;
    std::ifstream file(filenamex.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
        cout<<"File is openned!!\n";
    }

    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);

        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty() ) {
            std::ifstream isfile(path.c_str());
            if(!isfile){
                cout<<"no file: "<<path<<endl;
                continue;
            }
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }

}


faceDetector2::faceDetector2(QObject *parent) :  QThread(parent){

    face_founded=false;
    face_cascade_name = "C:/OpenCV/myBuild/opencv/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
    window_name = "Capture - Face detection";
    filenumber; // Number of file to be saved
    fn_haar = "C:/OpenCV/myBuild/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml";
    fn_csv = "myData.csv";
    if (!face_cascade.load(face_cascade_name))
    {
        printf("--(!)Error loading\n");
    };
    try {

           read_csv(fn_csv, images, labels);
       } catch (cv::Exception& e) {
           cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
           // nothing more we can do
           exit(1);
       }
}

void faceDetector2::run(){
    id=0;
    int tries=0;
    personnelTaken=false;
    cout<<"Reading..."<<endl;
   // Create a FaceRecognizer and train it on the given images:

   while (mainThread::search_for_face && (tries<5 || MainWindow::newUser)){
    cout<<"Try #"<<tries<<endl;
    detectAndDisplay();
    tries++;
   }
     if(tries==5) {
          emit isFaceFounded(false);
        }
      MainWindow::isProcessRunning=false;
      return;
}

void faceDetector2::recognizeFace(Mat &Frame){

    ofstream new_file("myData.csv");
    QString fileContent=
             MainWindow::imgPath+"\\s1\\1.pgm;0\n"
            +MainWindow::imgPath+"\\s1\\2.pgm;0\n"+MainWindow::imgPath+"\\s1\\3.pgm;0\n"
            +MainWindow::imgPath+"\\s2\\1.pgm;2\n"+MainWindow::imgPath+"\\s2\\2.pgm;2\n"
            +MainWindow::imgPath+"\\s2\\3.pgm;2\n"
            +MainWindow::imgPath+"\\s3\\1.pgm;3\n"+MainWindow::imgPath+"\\s3\\2.pgm;3\n"
            +MainWindow::imgPath+"\\s3\\3.pgm;3\n";
    new_file<<fileContent.toStdString();
    for(int i=0;i<=2;++i){
    QString fileName=MainWindow::imgPath+"\\"+MainWindow::foundedCode+"\\"+QString::number(i)+".png";
    new_file<<fileName.toStdString()<<";1\n";
    }
    new_file.close();
    read_csv(fn_csv, images, labels);
    cv::Ptr<cv::FaceRecognizer> model = createEigenFaceRecognizer();
    model->train(images, labels);
    int prediction;
    double confidence;
    model->predict(Frame,prediction,confidence);
    //Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat W = model->getMat("eigenvectors");
    // Get the sample mean from the training data
    Mat mean = model->getMat("mean");

    Mat evs = Mat(W, Range::all(), Range(0, 15));
    Mat projection = subspaceProject(evs, mean, Frame.reshape(1,1));
    Mat reconstruction = subspaceReconstruct(evs, mean, projection);
    normalize(reconstruction.reshape(1, Frame.rows),reconstruction,0,255,
                               NORM_MINMAX,CV_8UC1);

   double err=norm(reconstruction,Frame);
   err/=((double)Frame.cols*Frame.rows);
   cout<<"Prediction error:=> "<<err<<endl;
    // Normalize the result:
    //reconstruction = norm_0_255(reconstruction.reshape(1, testSample.rows));
    // Display or save:
    //cout<<Frame.rows<<"x"<<Frame.cols<<" :: "<<Frame.channels()<<"----------"<<endl;
    //double err=norm(reconstruction,Frame);
    //cout<<"Err: "<<err/((double)reconstruction.cols*reconstruction.rows)<<endl;

    //rectangle(original, face_i, CV_RGB(0, 255,0), 1);
              // Create the text we will annotate the box with:
    int originalClass=1;
    if(err<.6){
    QSqlQuery query;
    query.exec("select classID,attendantID from previously_registered where attendantID='"
               +MainWindow::foundedCode+"'");
    box_text="Not Found!";

    //while(query.next()){
    //box_text=query.value(1).toString().toStdString();
    //originalClass=query.value(0).toString().toInt();
    //emit authentication(true);
    //}
    if(prediction!=originalClass){
        emit authentication(false);
        box_text="Authentication Failed!";
    } else{
        box_text="Face verified";
        emit authentication(true);
    }
    } else{
        box_text="Authentication Failed";
        emit authentication(false);
    }
    uploadImage *upload = new uploadImage;
    QString filePath=MainWindow::foundedCode+".png";
    upload->uploadFile(filePath.toStdString(),MainWindow::foundedCode.toStdString(),
                       (int)(prediction==originalClass));
    cout<<"Prediction: "<<prediction<<" , Confidence: "<<confidence<<endl;

    emit sendOwnerName();

}


void faceDetector2::detectAndDisplay(){
    Mat Frame=barCodeThread::Frame;
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat res;
    Mat gray;


    cvtColor(Frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

// Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
// Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;
    cv::Rect roi_Personnel;
    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element
    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element
    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)
    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
            //if(1.2*roi_b.x<Frame.cols) roi_Personnel.x=ro_i.x;

        }

        crop = Frame(roi_b);
        if(MainWindow::newUser && !personnelTaken){
            roi_Personnel.x=.8*roi_b.x;
            roi_Personnel.y=.6*roi_b.y;
            roi_Personnel.width=1.2*roi_b.width;
            roi_Personnel.height=1.5*roi_b.height;
            Mat personnel_photo=Frame(roi_Personnel);

             imwrite((MainWindow::imgPath+"\\"+MainWindow::foundedCode
                      +"\\"+"personnel.png").toStdString(),personnel_photo);
             uploadImage *upload = new uploadImage(this);
             QString fileAddress=MainWindow::imgPath+"\\"+MainWindow::foundedCode
                                  +"\\"+"personnel.png";
             upload->uploadFile(fileAddress.toStdString()
                                ,MainWindow::foundedCode.toStdString(),
                                -1);

            personnelTaken=true;
             qDebug()<<"Personnel image"<<endl;

        }
        resize(crop, res, Size(112, 92), 0, 0, INTER_CUBIC); // This will be needed later while saving images
        cvtColor(res, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

        //Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        //Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        //rectangle(Frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    }

// Show image
    //sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
    //text = sstm.str();

    //putText(Frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
   // imshow("original", frame);

    if (!crop.empty())
    {
    //    imshow("detected", crop);
    if(gray.cols<90 || gray.rows<80) return;
        cout<<"Face Founded"<<endl;
        QImage img= QImage((const unsigned char*)(Frame.data),
        Frame.cols,Frame.rows,QImage::Format_RGB888);

        face_founded=true;
        QString filename;
        if(MainWindow::newUser){
            QDir dir(MainWindow::imgPath+"\\"+MainWindow::foundedCode);
            if (!dir.exists()){
                cout<<"File does not exist!"<<endl;
                QDir().mkdir(MainWindow::imgPath+"\\"+MainWindow::foundedCode);
            }
           QSqlQuery query;

           query.exec("select classID from previously_registered where attendantID='"
                      +MainWindow::imgPath+"\\"+MainWindow::foundedCode+"'");
           while(query.next()){
            currentClassID=query.value(0).toInt();
           }
        filename=MainWindow::imgPath+"\\"+MainWindow::foundedCode+"/"+QString::number(id)+".png";
        ofstream new_file("myData.csv",ios::app);
        new_file<<filename.toStdString()<<";"<<currentClassID<<"\n";
        new_file.close();
        }else{
        filename=MainWindow::imgPath+"\\"+MainWindow::foundedCode+".png";
        }


        imwrite(filename.toStdString(),gray);
        emit sendImage(img,MainWindow::foundedCode);

        emit sendCropped(filename);

        id++;
        cout<<"IMAGE #"<<id<<endl;
        if(MainWindow::newUser && id<4){
            barCodeThread::waitms=0;
            return;
        }
        if (id>=4) MainWindow::newUser=false;
        mainThread::search_for_face=false;

       recognizeFace(gray);

    }
    else{
         mainThread::search_for_face=true;
         face_founded=false;

      //  destroyWindow("detected");
    }

}
