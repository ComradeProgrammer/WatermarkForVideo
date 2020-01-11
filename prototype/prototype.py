import cv2
import numpy as np
import copy

class Picture:
    def __init__(self):
        self.pic=None

    def decToBin(self,information):
        binary=bin(information)
        binary=binary[2:]
        zeros="".join(['0']*(32-len(binary)))
        binary=zeros+binary
        return binary
    
    def decomposePicture(self):
        shape=self.pic.shape
        b = np.zeros((shape[0], shape[1]))
        g = np.zeros((shape[0], shape[1]))
        r = np.zeros((shape[0], shape[1])) 
        for i in range(0, shape[0]):
            for j in range(0, shape[1]):
                b[i][j] = self.pic[i][j][0]
                g[i][j] = self.pic[i][j][1]
                r[i][j] = self.pic[i][j][2]
        return (b,g,r)

    def composePicture(self,b,g,r):
        cnt=0
        shape=(b.shape[0],b.shape[1],3)
        self.pic=np.zeros(shape)
        for i in range(0, shape[0]):
            for j in range(0, shape[1]):
                if b[i][j]<0:
                    self.pic[i][j][0]=0
                    cnt+=1
                elif b[i][j]>255:
                    self.pic[i][j][0]=255
                    cnt+=1
                else:
                    self.pic[i][j][0]=b[i][j]

                if g[i][j]<0:
                    self.pic[i][j][1]=0
                    cnt+=1
                elif g[i][j]>255:
                    self.pic[i][j][1]=255
                    cnt+=1
                else:
                    self.pic[i][j][1]=g[i][j]

                if r[i][j]<0:
                    self.pic[i][j][2]=0
                    cnt+=1
                elif r[i][j]>255:
                    self.pic[i][j][2]=255
                    cnt+=1
                else:
                    self.pic[i][j][2]=r[i][j]
        print("overflow "+str(cnt))
        self.pic=self.pic.astype(np.uint8)
        return self.pic

    def loadPictureByName(self,fileName):
        self.pic=cv2.imread(fileName,cv2.IMREAD_COLOR)
       
    def loadPictureByMat(self,mat):
        self.pic=mat.copy()
        
    
    def generateInformationPicture(self,information,energy):
        binary=self.decToBin(information)
        mat=np.zeros((64,64,3))
        ptr=0
        for i in range(0,8):
            for j in range(0,4):
                bit= energy if binary[ptr]=="1" else 0
                for k in range(0,8):
                    for l in range(0,16):
                        mat[8*i+k][16*j+l][0]=bit
                        mat[8*i+k][16*j+l][1]=bit
                        mat[8*i+k][16*j+l][2]=bit
                ptr+=1
        # cv2.imshow("information picture",mat.astype(np.uint8))
        # cv2.waitKey(0)
        return mat

    def addWatermark(self,information,energy,biasx=32,biasy=32):
        mat=self.generateInformationPicture(information,energy)
        (b,g,r)=self.decomposePicture()
        bdct = cv2.dct(b)
        gdct = cv2.dct(g)
        rdct = cv2.dct(r)
        for i in range(0,64):
            for j in range(0,64):
                bdct[biasx+i][biasy+j]+=mat[i][j][0]
                gdct[biasx+i][biasy+j]+=mat[i][j][1]
                rdct[biasx+i][biasy+j]+=mat[i][j][2]
        newpic=Picture()
        nb=cv2.idct(bdct)
        ng=cv2.idct(gdct)
        nr=cv2.idct(rdct)
        newpic.composePicture(nb,ng,nr)
        # cv2.imshow("watermark added",newpic.pic.astype(np.uint8))
        # cv2.waitKey(0)
        return newpic

    def extractWatermark(self,originPicture,biasx=32,biasy=32,extractFactor=1):
        b2,g2,r2=self.decomposePicture()
        b,g,r=originPicture.decomposePicture()
        bdct2 = cv2.dct(b2)
        gdct2 = cv2.dct(g2)
        rdct2 = cv2.dct(r2)
        bdct=cv2.dct(b)
        gdct=cv2.dct(g)
        rdct=cv2.dct(r)
        mat2=np.zeros((64,64,3))
        for i in range(0,64):
            for j in range(0,64):
                mat2[i][j][0]=(bdct2[biasx+i][biasy+j]-bdct[biasx+i][biasy+j])*extractFactor
                mat2[i][j][1]=(gdct2[biasx+i][biasy+j]-gdct[biasx+i][biasy+j])*extractFactor
                mat2[i][j][2]=(rdct2[biasx+i][biasy+j]-rdct[biasx+i][biasy+j])*extractFactor
                if mat2[i][j][0]<0:
                    mat2[i][j][0]=0
                if mat2[i][j][1]<0:
                    mat2[i][j][1]=0
                if mat2[i][j][2]<0:
                    mat2[i][j][2]=0
                if mat2[i][j][0]>255:
                    mat2[i][j][0]=255
                if mat2[i][j][1]>255:
                    mat2[i][j][1]=255
                if mat2[i][j][2]>255:
                    mat2[i][j][2]=255
        # cv2.imshow("extracted information",mat2.astype(np.uint8))
        # cv2.waitKey(0)
        return mat2.astype(np.uint8)

class WatermarkAdder():
    def __init__(self,inputFileName,outputFileName,interval=1,energy=10,bias=416):
        self.inputvideo=inputFileName
        self.outputvideo=outputFileName
        self.interval=interval
        self.energy=energy
        self.bias=bias

    def addWatermarkOnFrame(self,frame,information):
        pic=Picture()
        pic.loadPictureByMat(frame)
        newpic=pic.addWatermark(information,self.energy,biasx=self.bias,biasy=self.bias)
        return newpic.pic.astype(np.uint8)
    
    def addWatermarkOnVideo(self):
        #prepare to read the new video
        videoCapture=cv2.VideoCapture(self.inputvideo)
        #fetch basic information of this video
        frameCount=int(videoCapture.get(cv2.CAP_PROP_FRAME_COUNT))
        fps=int(videoCapture.get(cv2.CAP_PROP_FPS))
        height=int(videoCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))
        width=int(videoCapture.get(cv2.CAP_PROP_FRAME_WIDTH))
        print("Input video has "+str(frameCount)+" frames")
        print("fps:"+str(fps)+" "+str(width)+"x"+str(height))
    
        #prepare to write back a new video
        videoWriter=cv2.VideoWriter(self.outputvideo,cv2.VideoWriter_fourcc(*'mp4v'),fps,(width,height))
        #TODO:new fps should be recalculated
        for i in range(0,frameCount):
            if i%self.interval==0:
                print("processing frame No"+str(i))
                ret,frame=videoCapture.read()
                newframe=self.addWatermarkOnFrame(frame,i)
                videoWriter.write(frame)
                videoWriter.write(newframe)
            else:
                ret,frame=videoCapture.read()
                videoWriter.write(frame)

class WatermarkExtractor:
    def __init__(self,interval,fileName,bias=416,extractFactor=6):
        self.bias=bias
        self.fileName=fileName
        self.extracFactor=extractFactor
        self.interval=interval
    
    def extractWatermarkOnFrame(self,frame,originalFrame):
        newpic=Picture()
        newpic.loadPictureByMat(frame)
        pic=Picture()
        pic.loadPictureByMat(originalFrame)

        m=newpic.extractWatermark(pic,self.bias,self.bias,self.extracFactor)
        cv2.imshow("extracted information",m)
        cv2.waitKey(0)
        return m

    def extractWatermarkOnVideo(self):
        videoCapture=cv2.VideoCapture(self.fileName)
        frameCount=int(videoCapture.get(cv2.CAP_PROP_FRAME_COUNT))
        fps=int(videoCapture.get(cv2.CAP_PROP_FPS))
        height=int(videoCapture.get(cv2.CAP_PROP_FRAME_HEIGHT))
        width=int(videoCapture.get(cv2.CAP_PROP_FRAME_WIDTH))
        print("Input video has "+str(frameCount)+" frames")
        print("fps:"+str(fps)+" "+str(width)+"x"+str(height))
        i=0
        while i< frameCount:
            if i%self.interval==0:
                ret,frame=videoCapture.read()
                ret2,frame2=videoCapture.read()
                m=self.extractWatermarkOnFrame(frame2,frame)
                # cv2.imshow("",m)
                # cv2.waitKey()
                i+=2
            else:
                ret,frame=videoCapture.read()
                i+=1
                


# p=Picture()
# p.loadPictureByName("xin.jpg")
# p2=p.addWatermark(555,10,biasx=512-64,biasy=512-64)
# m=p2.extractWatermark(p,extractFactor=6,biasx=512-64,biasy=512-64)
v=WatermarkAdder("testvideo.mp4","testout.mp4",interval= 27,energy= 20)
v.addWatermarkOnVideo()
v2=WatermarkExtractor(28,"testout.mp4")
v2.extractWatermarkOnVideo()




        




