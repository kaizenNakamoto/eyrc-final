#Team ID: 681
#Author List: Kiran R S
#Filename: imgstrip.py
#Theme: eYRC-2018 HOMECOMING
#Functions: middle(mid, c, hier, path), outer(outerz, c, hier, path), 
#           contours(img, type), transf(num, type), img_process(path,s_path), main()
#Global Variables: None


######################################################### MODULES REQUIRED #########################################################
import cv2
import argparse

''' This function takes in following arguments 
    mid : image, middle region of input
    c : contours
    hier : hierarchy
    path: path to save contours

    This function finds and draws contours'''


def middle(mid, c, hier, path):

    mid1 = cv2.copyMakeBorder(mid, 0, 0, 0, 0, cv2.BORDER_REPLICATE)
    inner_count=0
    habitats=list()

    # hier = [ NEXT, PREV, FIRST CHILD, PARENT]

    # Finding Squares with a child and finding the child with maximum area and drawing its contour

    for i in range(0,len(c)):
        if hier[0][i][3]==1:
            inner_count+=1
            if hier[0][i][2]!=-1:
                child=hier[0][i][2]
                max_child=hier[0][i][2]
                while(hier[0][child][0]!=-1):
                    next_child=hier[0][child][0]
                    if cv2.contourArea(c[next_child])>cv2.contourArea(c[max_child]):
                        max_child=next_child
                    child=next_child
                (x,y,w,h)=cv2.boundingRect(c[max_child])
                num = transf(inner_count, 'mid')
                num = str(num)
                imgg=mid1[y:y + h, x:x + w]
                cont = [num,imgg]
                habitats.append(cont)
                if path!=None:
                    name = path + num + '.png'
                    cont=mid1[y:y+h,x:x+w]
                    cv2.imwrite(name,cont)
                cv2.rectangle(mid,(x,y),(x+w,y+h),(0,0,255),2)
                cx=int(x+(w/2))
                cy=int(y+(h/2))
                cv2.putText(mid, num, (cx-36, cy+15), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 0,255), 4)
    return mid,habitats


''' This function takes in following arguments 
    outerz : image, the outer region of the input
    c : contours
    hier : hierarchy
    path: path to save contours
    This function draws contours'''


def outer(outerz, c, hier, path):
    outerz1= cv2.copyMakeBorder(outerz, 0, 0, 0, 0, cv2.BORDER_REPLICATE)
    animals=list()
    inner_count = 0

    # Finding Squares with a child and finding the child with maximum area and drawing its contour

    for i in range(0, len(c)):
        if hier[0][i][3] == 0:
            inner_layer = hier[0][i][2]
            inner_count += 1
            if hier[0][inner_layer][2] != -1:
                (x, y, w, h) = cv2.boundingRect(c[inner_layer])
                cx=int(x+(w/2))
                cy=int(y)
                lab=transf(inner_count,'outer')
                name=''
                imgg=outerz1[y:y + h, x:x + w]
                cont = [lab,imgg]
                animals.append(cont)
                if path!=None:
                    name = path + lab + '.png'
                    cont=outerz[y:y+h,x:x+w]
                    cv2.imwrite(name,cont)
                cv2.rectangle(outerz,(x,y),(x+w,y+h),(0,0,255),2)
                cv2.putText(outerz,lab,(cx-35,cy-10),cv2.FONT_HERSHEY_SIMPLEX,2,(0,0,255),4)
    return outerz,animals


''' This function takes in following arguments 
    img : image
    type : type of image i.e either middle or outer
    This function finds contours'''


def contours(img, type):
    # Middle image
    if (type == 'mid'):

        # Greyscaling
        gry = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Thresholding : op is source image pixel if image pixel > threshold value else pixel is replaced with black(0)
        _, thres = cv2.threshold(gry, 230, 255, cv2.THRESH_TOZERO)
        # Getting Contour
        c, hier = cv2.findContours(thres, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
        return c, hier
    # Outer image
    else:
        # Blurring for removing noise
        img = cv2.GaussianBlur(img, (1, 1), 0)
        gry = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        _, thres = cv2.threshold(gry, 100, 255, cv2.THRESH_TOZERO)
        c, hier = cv2.findContours(thres, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
        return c, hier


''' This function takes in following arguments 
    num: actual index
    type: Outer or middle image
    This function finds the custom index value'''


def transf(num, type):
    if (type == 'mid'):
        if (num >= 1 and num <= 5):
            return abs(num - 6)
        elif (num > 5 and num <= 10):
            return abs(num - 16)
        elif (num > 10 and num <= 15):
            return abs(num - 26)
        elif (num > 15 and num <= 20):
            return abs(num - 36)
        elif (num > 20 and num <= 25):
            return abs(num - 46)
    else:
        column = ['_', 'F', 'E', 'D', 'C', 'B', 'A']
        column2 = ['_', 'F', 'A']
        if (num >= 1 and num <= 6):
            row = '1'
            col = column[num]
        elif (num >= 7 and num <= 8):
            row = '2'
            col = column2[abs(num - 6)]
        elif (num >= 9 and num <= 10):
            row = '3'
            col = column2[abs(num - 8)]
        elif (num >= 11 and num <= 12):
            row = '4'
            col = column2[abs(num - 10)]
        elif (num >= 13 and num <= 14):
            row = '5'
            col = column2[abs(num - 12)]
        elif (num >= 15):
            row = '6'
            col = column[abs(num - 14)]
        label = col + row
        return label

def img_process(path,s_path=None):
    ################################################## PREPROCESSING OF IMAGE ############################################################

    # declaring output window
    im1 = cv2.imread(path)
    hi, wi, ch = im1.shape  # original size

    im = cv2.resize(im1, (2319, 2319))
    lx = 370
    ly = 370
    hx = 1945
    hy = 1945

    # Making copy of image
    outr = cv2.copyMakeBorder(im, 0, 0, 0, 0, cv2.BORDER_REPLICATE)

    # Extracting middle region of interest(ROI)
    mid = im[lx:hx, ly:hy]

    # Erasing Middle region
    outr = cv2.rectangle(outr, (lx, ly), (hx, hy), (255, 255, 255), -1)

    ###################################################### CONTOUR EXTRACTION #############################################

    # Getting the inner square contours and processing it
    c, hier = contours(mid, 'mid')
    mid,habitats = middle(mid, c, hier, path=None)

    # Getting the outer sqaure contours and processing it
    c, hier = contours(outr, 'outer')
    outr,animals = outer(outr, c, hier, path=None)

    # Placing middle region back into its original place
    outr[lx:hx, ly:hy] = mid

    # Resizing the image back to its original size
    final = cv2.resize(outr, (hi, wi))

    # If -s is passed below block is executed to save the image
    if (s_path!= None):
        cv2.imwrite(s_path, final)
    return final,animals,habitats

def main():
    ##################################################COMMAND LINE ARGUMENT PASS############################################################

    parser = argparse.ArgumentParser()
    parser.add_argument('path', help='Path to the image can be both relative or absolute')
    parser.add_argument('-s', '--save', help='Save parameter will save the finalimage with contours onto diskwith\
     filename as specified.')
    parser.add_argument('-c', '--cc', help='To cut the found contours and save them separately at the path specified')
    args = parser.parse_args()
    fin,_,_=img_process(args.path,args.save)
    cv2.imshow('output',fin)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()