#Team ID: 681
#Author List: Kiran R S
#Filename: main.py
#Theme: eYRC-2018 HOMECOMING
#Functions: predict_image(image, model), main()
#Global Variables: None



######################################################### MODULES REQUIRED #########################################################
import torch
import torchvision.transforms as transforms
from torch.autograd import Variable
from PIL import Image
import argparse
import imgstrip
import cv2
import mapp
'''
This function is used to pre-process an image and pass it to the model and get the class name
image = PIL image
mod = CNN model
'''
def predict_image(image,mod):

    # Loading the pre-trained model
    model=torch.load(mod)

    # Defining transforms for pre-processing of the input
    test_transform = transforms.Compose([
        transforms.Resize(224),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])])

    # Passing Tensor to model
    image_tensor = test_transform(image)
    image_tensor = image_tensor.unsqueeze(0)
    input = Variable(image_tensor.cuda())
    output = model(input)

    # Finding maximum index value position
    index = output.data.cuda().argmax()
    return index


######################################################################### MAIN FUCNTION ####################################################
def main():
    ##################################################COMMAND LINE ARGUMENT PASS############################################################
    parser = argparse.ArgumentParser(
        description="Identifying animals and habitats from predefined models")
    parser.add_argument('path', help='Path to the image can be both relative or absolute')
    parser.add_argument('-s', '--save', help='Save parameter will save the final image with contours onto diskwith\
      filename as specified.')
    parser.add_argument('--amod', default="./animal-model.pth", help="Use a trained animal model")
    parser.add_argument('--hmod', default="./habitat-model.pth", help="Use a trained habitat model")
    args = parser.parse_args()

    #############################################################VARIABLES##################################################################
    cv2.namedWindow('output',cv2.WINDOW_NORMAL)
    cv2.resizeWindow('output',(800,800))
    animal_labels=['arctic fox', 'bear', 'bee', 'butterfly', 'cat', 'cougar', 'cow', 'coyote', 'crab',
            'crocodile', 'deer', 'dog', 'eagle', 'elephant', 'fish', 'frog', 'giraffe',
            'goat', 'hippo', 'horse', 'kangaroo', 'lion', 'monkey', 'otter', 'panda',
            'parrot', 'penguin', 'raccoon', 'rat', 'seal', 'shark', 'sheep', 'skunk',
            'snake', 'snow leopard', 'tiger', 'yak', 'zebra']

    habitat_labels=['baseball', 'basketball court', 'beach', 'circular farm', 'cloud', 'commercial area',
            'dense residential','desert','forest','golf course','harbor','island',
            'lake','meadow','medium residential area','mountain','rectangular farm','river',
            'sea glacier','shrubs','snowberg','sparse residential area','thermal power station','wetland']

    output=dict()

    #Stripping child images from imgstrip.py
    '''
    fin=Processed image with contours and id marked
    animal = [[id,contour],...] list of the location and id of every animal contour image
    habitat = [[id,contour],...] list of the location and id of every habitat contour image
    
    '''
    fin, animal, habitat = imgstrip.img_process(args.path, args.save)

    # Calling models
    model_animal=args.amod
    model_habitat=args.hmod


    # Finding habitat names using model
    for x,i in enumerate(habitat):
        label = i[0]
        img=i[1]

        # Converting 4 channel image to 3 channel
        if len(img.shape) > 2 and img.shape[2] == 4:
            img = cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)

        # Converting BGR TO RGB (opencv format to PIL format)
        img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

        # Converting numpy image array to PIL image
        img=Image.fromarray(img)

        # Passing image to model to get name of image
        index = predict_image(img, model_habitat)
        name = habitat_labels[index]

        # Appending Final Dictionary
        output[label]=name


    # Finding animals names using model
    for i in animal:
        label = i[0]
        img=i[1]

        # Converting 4 channel image to 3 channel
        if len(img.shape) > 2 and img.shape[2] == 4:
            img = cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)

        # Converting BGR TO RGB (opencv format to PIL format)
        img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

        # Converting numpy image array to PIL image
        img=Image.fromarray(img)

        # Passing image to model to get name of image
        index = predict_image(img, model_animal)
        name = animal_labels[index]

        # Appending Final Dictionary
        output[label] = name

    # Displaying final output
    cv2.imshow('output',fin)
    
    mapp.do_mapping(output)
    # Printing final dictionary
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
