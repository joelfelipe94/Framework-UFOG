#include "colorlines.h"

ColorLines::ColorLines()
{
}
Vec3f ColorLines::findOrientation(Mat image)
{






    /// Remove the negatives and for some reason resize the small patches
    Mat m_resized;
    int w=image.cols;
    int h=image.rows;
    int max_dim=max(w,h);
    if(max_dim<600){
        double factor=max(600.0/(double)max_dim,2.0);
        resize(image, m_resized, Size(), factor, factor, INTER_LINEAR);
    }
    int w_r=m_resized.cols;
    int h_r=m_resized.rows;
    Mat m_gray, edges;
    m_resized.convertTo(m_gray, CV_8UC1);
        cvtColor(m_resized, m_gray, CV_BGR2GRAY);
    double otsu_thresh_val = cv::threshold(m_gray, edges, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    //nao sei como colocar esses parametros, o matlab passa um parametro so e eu nao sei como traduzir isso pro opencv
    Canny(m_gray, edges, otsu_thresh_val*0.5, otsu_thresh_val);




    /// Now we should already eliminate all patches that do not contain an edge.
    int n=10;

        int indLocs[w_r*h_r][2];
        int cont=0;
        int x, y;
        uchar *p_data=edges.data;
        int flagbreak=0;
        for(int i=0; i<=h_r-n; i++){
            for(int j=0; j<=w_r-n; j++){
                //printf("%d %d\n", i, j);
                flagbreak=0;
                for(int i2=i; i2<i+n-1; i2++){
                    for(int j2=j; j2<j+n-1; j2++){
                        if(p_data[w*i2+j2]){
                            flagbreak=1;
                            break;
                        }
                    }
                }
                if(!flagbreak){
                    indLocs[cont][0]=i;
                    indLocs[cont][1]=j;
                    cont++;
                }
            }
        }
        for(int i=0; i<cont; i++){
            printf("%d %d\n", indLocs[i][0]+1,indLocs[i][1]+1);
        }

}
