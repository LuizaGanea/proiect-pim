#import matplotlib.pyplot as plt
#import numpy as np
#import tensorflow as tf
#import tensorflow_datasets as tfds

#from keras import layers
#import keras
# import required image module
from PIL import Image, ImageEnhance
import glob
import os
# Open an already existing image
imageObject = Image.open("stop_sign.ppm")


k=1
sursa="imagini/back/"
destinatie="back2/"
for x in os.listdir(sursa):
    if x.endswith(".jpg") or x.endswith(".webp"):
        

        x=sursa+x
        nume= str(k)+".jpg"
        print(nume)
        
        im=Image.open(str(x))
        if im.mode != 'RGB':
            im = im.convert('RGB')
        w, h =im.size 
        if(h>800):
            ratio=800/h
            nh=h*ratio
            nw=w*ratio
            newsize = (int(nw),int(nh))
            im=im.resize(newsize)


        salvare=destinatie + str(k) +".jpg"
        im.save(salvare)
        k=k+1

        contrast= ImageEnhance.Contrast(im)
        temp= contrast.enhance(0.5)
        salvare=destinatie + str(k) +".jpg"
        temp.save(salvare)
        k=k+1

        contrast= ImageEnhance.Contrast(im)
        temp= contrast.enhance(2)
        salvare=destinatie + str(k) +".jpg"
        temp.save(salvare)
        k=k+1

        temp = im.rotate(10)
        salvare=destinatie + str(k) +".jpg"
        temp.save(salvare)
        k=k+1

        temp = im.rotate(-10)
        salvare=destinatie + str(k) +".jpg"
        temp.save(salvare)
        k=k+1


        
# Do a flip of left and right
#hori_flippedImage = imageObject.transpose(Image.FLIP_LEFT_RIGHT)

# Show the original image
#imageObject.show()

# Show the horizontal flipped image
#hori_flippedImage.show()

#hori_flippedImage.save("StopRotate.jpg")

#print(glob.glob("stop/*.ppm"))


#citire nume
#for x in os.listdir("stop/"):
    #if x.endswith(".ppm"):
    # Prints only text file present in My Folder
      #  print(x)

#////////////////////////////////////////////
#colorImage  = Image.open("00000_00001.ppm")
#colorImage.show()

#rotated     = colorImage.rotate(45)

#transposed  = colorImage.transpose(Image.ROTATE_90)

enhancer = ImageEnhance.Contrast(im)
constrastImg = enhancer.enhance(2)

constrastImg.show()

#colorImage.show()

#rotated.show()

#transposed.show()
    

