package exp.com.tum.opencvjniexperimental;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by munjalbharti on 07.05.16.
 */
public class Mesh {
    ArrayList<Vec3f> Vertices = null;
    ArrayList<Vec3f> TexCoords = null;
    ArrayList<Vec3f> Normals = null;
    ArrayList<FaceVert[]> Faces ;

    public Mesh(){

    }

    public Mesh( Vec3f[] VerticesArray,Vec3f[] TexCoordsArray,Vec3f[] NormalsArray,FaceVert[][] FacesArray){
        this.Vertices=new ArrayList<Vec3f>(Arrays.asList(VerticesArray));
        if(TexCoordsArray != null ) {
            this.TexCoords = new ArrayList<Vec3f>(Arrays.asList(TexCoordsArray));
        }

        if(NormalsArray != null) {
            this.Normals = new ArrayList<Vec3f>(Arrays.asList(NormalsArray));
        }

        this.Faces=new ArrayList<FaceVert[]>();

        for(int i=0;i < FacesArray.length;i++){
            FaceVert[] fArray=FacesArray[i];
            this.Faces.add(fArray);
        }


    }


    public ArrayList<Vec3f> getVertices(){
        return Vertices;
    }

    public void setVertices(ArrayList<Vec3f> Vertices){
        this.Vertices=Vertices;
    }

    public ArrayList<Vec3f> getTexCoords(){
        return TexCoords;
    }

    public void setTexCoords(ArrayList<Vec3f> TexCoords){
        this.TexCoords=TexCoords;
    }

    public ArrayList<Vec3f> getNormals(){
        return Normals;
    }
    public void setNormals(ArrayList<Vec3f> Normals){
        this.Normals=Normals;
    }

    public ArrayList<FaceVert[]> getFaces(){
        return Faces;
    }

    public void setFaces(ArrayList<FaceVert[]> Faces){
        this.Faces=Faces;
    }




}
