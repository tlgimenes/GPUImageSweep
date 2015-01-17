#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "image.h"
#include <math.h>
#include "string.h"

using namespace std;

struct Camera {
	Matx33d A;
	Vec3d b;
	void read(string name) {
		ifstream f;
		f.open(name.c_str());
		if (!f.is_open()) {
			cout << "Erreur lecture fichier camera" << endl;
			return;
		}
		for (int i=0;i<3;i++)
			f >> A(i,0) >>A(i,1) >>A(i,2) >> b[i]; 
		f.close();
	}
	void print() const {
		cout << "A= " << A << endl 
			<< "b= " << b << endl;
	}
	Vec3d center() const {
		return Vec3d((-A.inv()*b).val);
	}
	Vec3d proj(const Vec3d& M) const {
		return Vec3d((A*M+b).val);
	}
	Vec3d normal(){
		Vec3d normal=Vec3d(A(2,0),A(2,1),A(2,2));
		return normal/norm(normal);                   //norm
	}
};


struct Data {
	Image<Vec3b> I1,I2;
	Image<float> F1,F2;
	Camera C1,C2;
	Matx33d F;
	Vec3d middleVector(){
		Vec3d n1=C1.normal();
		Vec3d n2=C2.normal();
		//return (n1+n2)/norm(n1+n2);
		return n1;
	}
};

struct Plane{
	double d;
	Vec3d normal;
};

Vec3d getMatch(const Vec3d& point2d, const Plane& P, const Camera& C1, const Camera& C2){         //point2d belongs to C1
	double lambda;
	Vec3d normal=P.normal;
	Vec<double,1> tmp=P.normal.t()*(C1.A.inv()*C1.b + C1.center());
	lambda=tmp(0)+P.d;
	tmp=P.normal.t()*C1.A.inv()*point2d;
	lambda/=tmp(0);
	Vec3d result=C2.A*(lambda*C1.A.inv()*point2d-C1.A.inv()*C1.b)+C2.b;
	return result/=result(2);
	
}

Matx33d homography(const Plane& P, const Camera& C1, const Camera& C2){    
	/*
	* P plan sur lequel on projete, à distance P.d du centre de la premiere camera C1.center(), et de normale P.normal
	*
	*
	*/
	Matx33d result;
	double lambda;
	Vec3d normal=P.normal;
	Vec<double,1> tmp=P.normal.t()*(C1.A.inv()*C1.b + C1.center());
	lambda=tmp(0)+P.d;
	result = lambda*C1.A.inv();
	result=result-C1.A.inv()*C1.b*P.normal.t()*C1.A.inv();
	result=C2.A*result;
	result=result + C2.b*P.normal.t()*C1.A.inv();
	return result;
}


int main( int argc, char** argv )
{
	Data D;
	D.I1=imread("../samples/visualize/08.jpg");
	D.I2=imread("../samples/visualize/09.jpg");
	imshow("I1",D.I1);
	imshow("I2",D.I2);

	D.C1.read("../samples/calib/matrix08.txt");
	D.C2.read("../samples/calib/matrix09.txt");
	D.C1.print();
	D.C2.print();

    std::cout << "center1 " << D.C1.center() << std::endl;
    std::cout << "center2 " << D.C2.center() << std::endl;

	Image<uchar>G1,G2;
	cvtColor(D.I1,G1,CV_BGR2GRAY);
	cvtColor(D.I2,G2,CV_BGR2GRAY);
	G1.convertTo(D.F1,CV_32F);
	G2.convertTo(D.F2,CV_32F);

	//définitions des plans

	Plane P1;
	P1.d=10;
	P1.normal=D.middleVector();
	

	//testing
	int n=4; //ncc parameter
	double thresh=-1.0;
	Plane P;
	//P.d=298;
	P.normal=D.middleVector();
	Mat correlation = Mat::zeros( D.F1.size(), CV_32FC1 );
	Mat matchPlane=Mat::zeros( D.F1.size(), CV_32FC1 );
	int minp=2370;
	int maxp=4000;
	for(int p=minp;p<maxp;p++){
		P.d=p;
		//cout<<P.d<<endl;
		Matx33d hm=homography(P,D.C1,D.C2);
        //std::cout << hm << std::endl;
		for(int i=n;i<D.F1.rows-n;i++){
			for(int j=n;j<D.F1.cols-n;j++){
				Vec3d img=hm*Vec3d(i,j,1);
				img/=img(2);
				int l= floor(img(0));
				int k= floor(img(1));

				if( l>=n && l< D.F1.rows-n && k >= n && k < D.F1.cols-n){
					double uc=NCC(D.F1, Point(j,i), D.F2, Point(k,l),n);
					if(uc> correlation.at<float>(i,j)&&uc>thresh){
						correlation.at<float>(i,j)=uc;
						matchPlane.at<float>(i,j)=P.d;
					}
				}

			}
		}
	}
	Mat matchPlane_norm,matchPlane_norm_scaled;
	normalize( matchPlane, matchPlane_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( matchPlane_norm, matchPlane_norm_scaled );

	imshow("planes",matchPlane_norm_scaled);

	//imwrite( "../200-350-normalI1-n4-t05.jpg", matchPlane_norm_scaled);

	waitKey(0);
	return 0;
}
