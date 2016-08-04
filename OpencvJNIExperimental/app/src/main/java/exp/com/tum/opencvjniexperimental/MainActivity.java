package exp.com.tum.opencvjniexperimental;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfKeyPoint;
import org.opencv.core.Scalar;
import org.opencv.features2d.FeatureDetector;
import org.opencv.features2d.Features2d;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2, View.OnTouchListener {

    private static final String  TAG = "Sample::Puzzle15::Activity";
    java.util.Map<MenuItem, Runnable> OptionsMenu;

    private CameraBridgeViewBase mOpenCvCameraView;
    public native int convertNativeGray(long matAddrRgba, long matAddrGray);
    public native String helloWorld();

    public native Mesh getMesh();
    public native int dvoFunc() ;
    public native int myDVOFunc();

    private ObjectView TheObjectView;
    private Boolean objectDisplayed=false;

    final java.security.SecureRandom Random = new java.security.SecureRandom();
      /* why use anything less */

    boolean meshDisplayed =false;

    float Rand()
      /* returns a random float in [0.0 .. 1.0). */
    {
        final byte[] V = new byte[4];
        Random.nextBytes(V);
        return
                (float)(
                        ((int)V[0] & 255)
                                |
                                ((int)V[1] & 255) << 8
                                |
                                ((int)V[2] & 255) << 16
                                |
                                ((int)V[3] & 255) << 24
                )
                        /
                        4294967296.0f;
    } /*Rand*/


    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {

        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "loaded");
                    /* Now enable camera view to start receiving frames */
                    mOpenCvCameraView.setOnTouchListener(MainActivity.this);
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        setTheme(R.style.AppTheme);
        setTitle("Reconstruction");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
       // getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

     //   getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
      //          WindowManager.LayoutParams.FLAG_FULLSCREEN);
        Log.d(TAG, "Creating and setting view");
        //mOpenCvCameraView = (CameraBridgeViewBase) new JavaCameraView(this, -1);
       // setContentView(mOpenCvCameraView);


        setContentView(R.layout.activity_main);

        mOpenCvCameraView = (CameraBridgeViewBase)findViewById(R.id.HelloOpenCvView);
        TheObjectView = (ObjectView)findViewById(R.id.object_view);

       // Mesh mesh = getCubeMesh();
       // ObjReader.Model Result = ObjReader.ReadMesh(mesh);
       // TheObjectView.SetObject(Result);

       // mOpenCvCameraView.setVisibility(View.INVISIBLE);

        mOpenCvCameraView.setCvCameraViewListener(this);


        // findViewById(R.id.object_view).setVisibility(View.INVISIBLE);


    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
            // System.loadLibrary("wow");
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
            System.loadLibrary("wow");
        }
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    private interface SelectedIDAction
    {
        public void Set(int SelectedID);
    } /*SelectedIDAction*/


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");

        OptionsMenu = new java.util.HashMap<MenuItem, Runnable>();

        OptionsMenu.put(menu.add(R.string.reset_view),
                        new Runnable()
                        {
                            public void run()
                            {
                                TheObjectView.ResetOrientation(true);
                            } /*run*/
                        } /*Runnable*/
                );
        OptionsMenu.put
                (
                        menu.add(R.string.options_lighting),
                        new Runnable()
                        {
                            public void run()
                            {
                                new OptionsDialog
                                        (MainActivity.this, getString(R.string.lighting_title),
                                                new SelectedIDAction()
                                                {
                                                    public void Set
                                                            (
                                                                    int SelectedID
                                                            )
                                                    {
                                                        TheObjectView.SetUseLighting(SelectedID != 0);
                                                    } /*Set*/
                                                } /*SelectedIDAction*/,
                        /*InitialButtonID =*/ TheObjectView.GetUseLighting() ? 1 : 0
                                        )
                                        .AddButton(getString(R.string.on), 1)
                                        .AddButton(getString(R.string.off), 0)
                                        .show();
                            } /*run*/
                        } /*Runnable*/
                );
        OptionsMenu.put
                (
                        menu.add(R.string.options_orient_faces),
                        new Runnable()
                        {
                            public void run()
                            {
                                new OptionsDialog
                                        (
                        /*ctx =*/ MainActivity.this,
                        /*Title =*/ getString(R.string.orient_faces_title),
                        /*Action =*/
                                                new SelectedIDAction()
                                                {
                                                    public void Set
                                                            (
                                                                    int SelectedID
                                                            )
                                                    {
                                                        TheObjectView.SetClockwiseFaces(SelectedID != 0);
                                                    } /*Set*/
                                                } /*SelectedIDAction*/,
                        /*InitialButtonID =*/ TheObjectView.GetClockwiseFaces() ? 1 : 0
                                        )
                                        .AddButton(getString(R.string.anticlockwise), 0)
                                        .AddButton(getString(R.string.clockwise), 1)
                                        .show();
                            } /*run*/
                        } /*Runnable*/
                );
        OptionsMenu.put
                (
                        menu.add(R.string.random_view),
                        new Runnable() {
                            public void run() {
                                final float
                                        X = Rand(),
                                        Y = Rand(),
                                        Z = Rand(),
                                        R = (float) Math.sqrt(X * X + Y * Y + Z * Z),
                                        Angle = Rand() * 360.0f;
                                TheObjectView.SetOrientation
                                        (
                                                new exp.com.tum.opencvjniexperimental.Rotation(Angle, true, X / R, Y / R, Z / R),
                                                true
                                        );
                            } /*run*/
                        } /*Runnable*/
                );

     //   MenuInflater inflater = getMenuInflater();
      //  inflater.inflate(R.menu.menu_main, menu);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        boolean Handled = false;
        final Runnable Action = OptionsMenu.get(item);
        if (Action != null)
        {
            Action.run();
            Handled = true;
        } /*if*/
        return Handled;


    }

    public void onCameraViewStarted(int width, int height) {
        System.out.println(width);
    }

    public void onCameraViewStopped() {
    }


    private class OptionsDialog
            extends android.app.Dialog
            implements android.content.DialogInterface.OnDismissListener
    {
        private final android.content.Context ctx;
        private final String Title;
        private final SelectedIDAction Action;
        private final int InitialButtonID;
        private class ButtonDef
        {
            final String ButtonTitle;
            final int ButtonID;

            public ButtonDef
                    (
                            String ButtonTitle,
                            int ButtonID
                    )
            {
                this.ButtonTitle = ButtonTitle;
                this.ButtonID = ButtonID;
            } /*ButtonDef*/
        } /*ButtonDef*/
        private final ArrayList<ButtonDef> TheButtonDefs =
                new ArrayList<ButtonDef>();
        private android.widget.RadioGroup TheButtons;

        public OptionsDialog(android.content.Context ctx, String Title, SelectedIDAction Action, int InitialButtonID)
        {
            super(ctx);
            this.ctx = ctx;
            this.Title = Title;
            this.Action = Action;
            this.InitialButtonID = InitialButtonID;
        } /*OptionsDialog*/

        public OptionsDialog AddButton(String ButtonTitle, int ButtonID)
        {
            TheButtonDefs.add(new ButtonDef(ButtonTitle, ButtonID));
            return
                    this;
        } /*AddButton*/

        @Override
        public void onCreate(Bundle ToRestore)
        {
            setTitle(Title);
            final android.widget.LinearLayout MainLayout = new android.widget.LinearLayout(ctx);
            MainLayout.setOrientation(android.widget.LinearLayout.VERTICAL);
            setContentView(MainLayout);
            TheButtons = new android.widget.RadioGroup(ctx);
            final android.view.ViewGroup.LayoutParams ButtonLayout =
                    new android.view.ViewGroup.LayoutParams
                            (
                                    android.view.ViewGroup.LayoutParams.FILL_PARENT,
                                    android.view.ViewGroup.LayoutParams.WRAP_CONTENT
                            );
            for (ButtonDef ThisButtonDef : TheButtonDefs)
            {
                final android.widget.RadioButton ThisButton =
                        new android.widget.RadioButton(ctx);
                ThisButton.setText(ThisButtonDef.ButtonTitle);
                ThisButton.setId(ThisButtonDef.ButtonID);
                TheButtons.addView(ThisButton, TheButtons.getChildCount(), ButtonLayout);
            } /*for*/
            MainLayout.addView(TheButtons, ButtonLayout);
            TheButtons.check(InitialButtonID);
            setOnDismissListener(this);
        } /*onCreate*/

        @Override
        public void onDismiss
                (
                        android.content.DialogInterface TheDialog
                )
        {
            Action.Set(TheButtons.getCheckedRadioButtonId());
        } /*onDismiss*/

    } /*OptionsDialog*/

    private Mesh getCubeMesh(){
        Mesh mesh=new Mesh();
        ArrayList<Vec3f> Vertices = new ArrayList<Vec3f>();
        //8 vertices
        Vertices.add(new Vec3f(0,0,0));
        Vertices.add(new Vec3f(0,0,1));
        Vertices.add(new Vec3f(0,1,0));
        Vertices.add(new Vec3f(0,1,1));
        Vertices.add(new Vec3f(1,0,0));
        Vertices.add(new Vec3f(1,0,1));
        Vertices.add(new Vec3f(1,1,0));
        Vertices.add(new Vec3f(1,1,1));



        //ArrayList<Vec3f> TexCoords = new ArrayList<Vec3f>();

        ArrayList<Vec3f> Normals = new ArrayList<Vec3f>();
        //6 normals
        Normals.add(new Vec3f(0,0,1));
        Normals.add(new Vec3f(0,0,-1));
        Normals.add(new Vec3f(0,1,0));
        Normals.add(new Vec3f(0,-1,0));
        Normals.add(new Vec3f(1,0,0));
        Normals.add(new Vec3f(-1,0,0));


        ArrayList<FaceVert[]> Faces =new ArrayList<FaceVert[]>() ;



        ArrayList<FaceVert> FaceVerts1 = new ArrayList<FaceVert>();
        FaceVerts1.add(new FaceVert( 1, null, 2));
        FaceVerts1.add(new FaceVert( 7, null, 2));
        FaceVerts1.add(new FaceVert( 5, null, 2));
        Faces.add(FaceVerts1.toArray(new FaceVert[FaceVerts1.size()]));

        ArrayList<FaceVert> FaceVerts2 = new ArrayList<FaceVert>();
        FaceVerts2.add(new FaceVert( 1, null, 2));
        FaceVerts2.add(new FaceVert( 3, null, 2));
        FaceVerts2.add(new FaceVert( 7, null, 2));
        Faces.add(FaceVerts2.toArray(new FaceVert[FaceVerts2.size()]));

        ArrayList<FaceVert> FaceVerts3 = new ArrayList<FaceVert>();
        FaceVerts3.add(new FaceVert( 1, null, 6));
        FaceVerts3.add(new FaceVert( 4, null, 6));
        FaceVerts3.add(new FaceVert( 3, null, 6));
        Faces.add(FaceVerts3.toArray(new FaceVert[FaceVerts3.size()]));

        ArrayList<FaceVert> FaceVerts4 = new ArrayList<FaceVert>();
        FaceVerts4.add(new FaceVert( 1, null, 6));
        FaceVerts4.add(new FaceVert( 2, null, 6));
        FaceVerts4.add(new FaceVert( 4, null, 6));
        Faces.add(FaceVerts4.toArray(new FaceVert[FaceVerts4.size()]));

        ArrayList<FaceVert> FaceVerts5 = new ArrayList<FaceVert>();
        FaceVerts5.add(new FaceVert( 3, null, 3));
        FaceVerts5.add(new FaceVert( 8, null, 3));
        FaceVerts5.add(new FaceVert( 7, null, 3));
        Faces.add(FaceVerts5.toArray(new FaceVert[FaceVerts5.size()]));

        ArrayList<FaceVert> FaceVerts6 = new ArrayList<FaceVert>();
        FaceVerts6.add(new FaceVert( 3, null, 3));
        FaceVerts6.add(new FaceVert( 4, null, 3));
        FaceVerts6.add(new FaceVert( 8, null, 3));
        Faces.add(FaceVerts6.toArray(new FaceVert[FaceVerts6.size()]));

        ArrayList<FaceVert> FaceVerts7 = new ArrayList<FaceVert>();
        FaceVerts7.add(new FaceVert( 5, null, 5));
        FaceVerts7.add(new FaceVert( 7, null, 5));
        FaceVerts7.add(new FaceVert( 8, null, 5));
        Faces.add(FaceVerts7.toArray(new FaceVert[FaceVerts7.size()]));

        ArrayList<FaceVert> FaceVerts8 = new ArrayList<FaceVert>();
        FaceVerts8.add(new FaceVert( 5, null, 5));
        FaceVerts8.add(new FaceVert( 8, null, 5));
        FaceVerts8.add(new FaceVert( 6, null, 5));
        Faces.add(FaceVerts8.toArray(new FaceVert[FaceVerts8.size()]));

        ArrayList<FaceVert> FaceVerts9 = new ArrayList<FaceVert>();
        FaceVerts9.add(new FaceVert( 1, null, 4));
        FaceVerts9.add(new FaceVert( 5, null, 4));
        FaceVerts9.add(new FaceVert( 6, null, 4));
        Faces.add(FaceVerts9.toArray(new FaceVert[FaceVerts9.size()]));

        ArrayList<FaceVert> FaceVerts10 = new ArrayList<FaceVert>();
        FaceVerts10.add(new FaceVert( 1, null, 4));
        FaceVerts10.add(new FaceVert( 6, null, 4));
        FaceVerts10.add(new FaceVert( 2, null, 4));
        Faces.add(FaceVerts10.toArray(new FaceVert[FaceVerts10.size()]));

        ArrayList<FaceVert> FaceVerts11 = new ArrayList<FaceVert>();
        FaceVerts11.add(new FaceVert( 2, null, 1));
        FaceVerts11.add(new FaceVert( 6, null, 1));
        FaceVerts11.add(new FaceVert( 8, null, 1));
        Faces.add(FaceVerts11.toArray(new FaceVert[FaceVerts11.size()]));

        ArrayList<FaceVert> FaceVerts12 = new ArrayList<FaceVert>();
        FaceVerts12.add(new FaceVert( 2, null, 1));
        FaceVerts12.add(new FaceVert( 8, null, 1));
        FaceVerts12.add(new FaceVert( 4, null, 1));
        Faces.add(FaceVerts12.toArray(new FaceVert[FaceVerts12.size()]));



        mesh.setVertices(Vertices);
        mesh.setTexCoords(null);
        mesh.setNormals(Normals);
        mesh.setFaces(Faces);

        return mesh;
    }


    private Mesh getTriangleMesh(){
        Mesh mesh=new Mesh();
        ArrayList<Vec3f> Vertices = new ArrayList<Vec3f>();
        //8 vertices
        Vertices.add(new Vec3f(0,0,0));
        Vertices.add(new Vec3f(5,(float)2.5,0));
        Vertices.add(new Vec3f(0,5,0));



        ArrayList<FaceVert[]> Faces =new ArrayList<FaceVert[]>() ;

        ArrayList<FaceVert> FaceVerts1 = new ArrayList<FaceVert>();
        FaceVerts1.add(new FaceVert( 1, null, null));
        FaceVerts1.add(new FaceVert( 2, null, null));
        FaceVerts1.add(new FaceVert( 3, null, null));
        Faces.add(FaceVerts1.toArray(new FaceVert[FaceVerts1.size()]));


        mesh.setVertices(Vertices);
        mesh.setTexCoords(null);
        mesh.setNormals(null);
        mesh.setFaces(Faces);

        return mesh;
    }



    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame cvCameraViewFrame) {
        //String msg=helloWorld();
        //System.out.println(msg);


        Mat mRgba = cvCameraViewFrame.rgba();

        //  Mat outImage ;
        // outImage=  displayFeaturePoints(mRgba);
        // return outImage;
        Mat mGray=new Mat();

        //using c++ opencv
//
        convertNativeGray(mRgba.getNativeObjAddr(), mGray.getNativeObjAddr());
        //Mesh mesh=getCubeMesh();
        //ObjReader.Model Result = ObjReader.ReadMesh(mesh);
        //TheObjectView.SetObject(Result);

        //TheObjectView.ResetOrientation(true);

      //  setContentView(R.layout.activity_main);

       // TheObjectView = (ObjectView)findViewById(R.id.object_view);
        if(!objectDisplayed) {
            //Mesh mesh = getCubeMesh();
           // Mesh mesh=getTriangleMesh();
            Mesh mesh=getMesh();
            ObjReader.Model Result = ObjReader.ReadMesh(mesh);
            TheObjectView.SetObject(Result);

           // int c=dvoFunc();
           int c=myDVOFunc();
           // System.out.print(c);
            objectDisplayed=true;
        }

        return mRgba;
    }

    public boolean onTouch(View view, MotionEvent event) {
        return true ;}


    public Mat displayFeaturePoints(Mat inputFrame){
        Mat grayPixel = new Mat(1,1, CvType.CV_8UC1);
        Imgproc.cvtColor(inputFrame, grayPixel, Imgproc.COLOR_BGR2GRAY);
        FeatureDetector detector= FeatureDetector.create(FeatureDetector.HARRIS);
        MatOfKeyPoint keypoints = new MatOfKeyPoint();
        detector.detect(grayPixel, keypoints);


        // Draw kewpoints
        Mat outputImage = new Mat();
        Scalar color = new Scalar(255, 0, 0); // BGR
        int flags = Features2d.DRAW_RICH_KEYPOINTS; // For each keypoint, the circle around keypoint with keypoint size and orientation will be drawn.
        Features2d.drawKeypoints(grayPixel, keypoints, outputImage, color, flags);
        //displayImage(Mat2BufferedImage(outputImage), "Feautures_"+detectorType);
        return outputImage ;

    }

}