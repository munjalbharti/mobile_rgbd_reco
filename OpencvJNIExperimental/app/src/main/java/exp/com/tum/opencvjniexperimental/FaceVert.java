/**
 * Created by munjalbharti on 04.05.16.
 */

package exp.com.tum.opencvjniexperimental;

public class FaceVert
{
    public Integer VertIndex, TexCoordIndex, NormalIndex;

    //called from jni..null is passed as 0
    public FaceVert(int VertIndex, int TexCoordIndex, int NormalIndex)
    {
        this.VertIndex = VertIndex;
        this.TexCoordIndex = TexCoordIndex;
        this.NormalIndex = NormalIndex;
    }

    //called from java
    public FaceVert(Integer VertIndex, Integer TexCoordIndex, Integer NormalIndex)
    {
        this.VertIndex = VertIndex;
        this.TexCoordIndex = TexCoordIndex;
        this.NormalIndex = NormalIndex;
    }

}