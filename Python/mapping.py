#Team ID: 681
#Author List: Kiran R S, Karthik S Nayak
#Filename: mapping.py
#Theme: eYRC-2018 HOMECOMING
#Functions: do_mapping(dic)
#Global Variables: file, df


import pandas as pd
import numpy as np
import time

#Read excel file containing the animal-habitat mapping
file = 'Animal_Habitat_Mapping.xlsx'
# Read file as dataframe using pandas
df = pd.read_excel(file, sheet_name=0, index_col=0)

'''
This functtion is used to perform the mapping between animals and associated habitats

'''
def do_mapping(dic):

        animal_labels = ['arctic fox', 'bear', 'bee', 'butterfly', 'cat', 'cougar', 'cow', 'coyote', 'crab',
                'crocodile', 'deer', 'dog', 'eagle', 'elephant', 'fish', 'frog', 'giraffe',
                'goat', 'hippo', 'horse', 'kangaroo', 'lion', 'monkey', 'otter', 'panda',
                'parrot', 'penguin', 'raccoon', 'rat', 'seal', 'shark', 'sheep', 'skunk',
                'snake', 'snow leopard', 'tiger', 'yak', 'zebra']

        invalid = {}
        valid = {}
        habitats = []
        animal_dict = {}
        habitat_dict = {}

        for key, value in dic.items():
                if key[0].isalpha():
                        animal_dict.update({key: value})
                else:
                        habitat_dict.update({key: value})

        #animal locations stored as a list
        aloc = list(animal_dict.keys())
        #animals in the arena stored as a list
        ani = list(animal_dict.values())
        #make a copy of the animals list
        anic = list(animal_dict.values())

        # Classify invalid and valid habitats
        for k, v in habitat_dict.items():
                if df.loc[v].isnull().all():
                        invalid.update({k: v})
                else:
                        valid.update({k: v})

        # Extract the valid habitat cell locations into a list
        valid_cell = list(valid.keys())
        # Extract the valid habitats in the arena into a list
        valid_hab = list(valid.values())


        occupied = np.zeros(23)

        #To extract the animals from excel file which appear exactly once in the whole file
        pref = []

        occur = []
        occ = []

        for index, row in df.iterrows():
                for j in df.columns.values:
                        occur.append(row[j])

        for v in occur:
                if str(v) != 'nan':
                        occ.append(v)

        for a in animal_labels:
                p = occ.count(a)
                if p == 1:
                        pref.append(a)

        newa = list()
        newk = list()
        tempa = list()
        tempk = list()
        for ia, an in enumerate(ani):
                if(an in pref):
                        newa.append(an)
                        newk.append(aloc[ia])
                        anic.remove(an)
                else:
                        tempa.append(an)
                        tempk.append(aloc[ia])

        n_ani = newa+tempa
        nloc = newk+tempk
        a = []
        h = []
        for i1, an in enumerate(n_ani):
                for i2, hb in enumerate(valid_hab):
                        if an in list(df.loc[hb]) and occupied[i2] <= 1:
                                a.append(nloc[i1])
                                h.append(valid_cell[i2])
                                occupied[i2] += 1
                                break

        print("-------------------")
        print("Animal Association")
        print("-------------------")
        for i1, a1 in enumerate(n_ani):
                print("%s(%s)" %(a1, nloc[i1]))
                time.sleep(0.5)
        print("-------------------")
        print("Habitat Association")
        print("-------------------")
        for j1, h1 in habitat_dict.items():
                if h1 in list(invalid.values()):
                        print("%s(%s)-Invalid" %(h1, j1))
                        time.sleep(0.5)
                else:
                        print("%s(%s)" %(h1, j1))
                        time.sleep(0.5)

        if('A1' in a):
                ind=0
                for inn,animal in enumerate(a):
                        if(animal=='A1'):
                                ind=inn
                                break
                temp=a[0]
                a[0]=a[ind]
                a[ind]=temp
                temp=h[0]
                h[0]=h[ind]
                h[ind]=temp
        print("-------------------")
        #final animals list
        print("Animals = %s" %(a))
        print("-------------------")
        #final habitats list
        print("Habitats = %s" %(h))
        print("-------------------")

        final_animals = ", ".join(a)
        final_habitats = ", ".join(h)

        #Write the lists to a file to be sent to the Firebird 5 Robot
        with open('final.txt', 'w') as f:
                for j in final_habitats:
                        f.write(j)
                f.write("\n")
                for i in final_animals:
                        f.write(i)

if __name__ == "__main__":
        pass