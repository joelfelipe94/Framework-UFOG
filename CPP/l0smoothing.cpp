#include "l0smoothing.h"

L0smoothing::L0smoothing()
{
}

Mat L0smoothing::doFilter(Mat image)
{


/*
    if ~exist('kappa','var')
        kappa = 2.0;
    end
    if ~exist('lambda','var')
        lambda = 2e-2;
    end
*/
/*
    S = im2double(Im);
*/
    float betamax = 1e5;

    //int fx1 = [1, -1];
    //int fy = [1; -1];
    //[N,M,D] = size(Im);
    //sizeI2D = [N,M];
    //otfFx = psf2otf(fx,sizeI2D);
    //otfFy = psf2otf(fy,sizeI2D);

    /// First we convert the image to a double scale
    image.convertTo(image, CV_32F);

    Mat amp,phase;
    fft( image,  amp,  phase);

    /// Probably Normin 1 is the amplitude.
   // Normin1 = fft2(S);
/*
    Denormin2 = abs(otfFx).^2 + abs(otfFy ).^2;
    if D>1
        Denormin2 = repmat(Denormin2,[1,1,D]);
    end
    beta = 2*lambda;
    while(beta < betamax){

        Denormin   = 1 + beta*Denormin2;
        % h-v subproblem
        h = [diff(S,1,2), S(:,1,:) - S(:,end,:)];
        v = [diff(S,1,1); S(1,:,:) - S(end,:,:)];
        if D==1
            t = (h.^2+v.^2)<lambda/beta;
        else
            t = sum((h.^2+v.^2),3)<lambda/beta;
            t = repmat(t,[1,1,D]);
        end
        h(t)=0; v(t)=0;
        % S subproblem
        Normin2 = [h(:,end,:) - h(:, 1,:), -diff(h,1,2)];
        Normin2 = Normin2 + [v(end,:,:) - v(1, :,:); -diff(v,1,1)];
        FS = (Normin1 + beta*fft2(Normin2))./Denormin;
        S = real(ifft2(FS));
        beta = beta*kappa;
        fprintf('.');
    }
    fprintf('\n');
    end
*/
    return image;

}
