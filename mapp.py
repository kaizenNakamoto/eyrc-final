import pandas as pd
import numpy as np
import time


file = 'Animal_Habitat_Mapping.xlsx'
df = pd.read_excel(file, sheet_name=0, index_col=0)

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

        aloc = list(animal_dict.keys())
        ani = list(animal_dict.values())
        anic = list(animal_dict.values())  # Copied Version


        for k, v in habitat_dict.items():
                if df.loc[v].isnull().all():
                        invalid.update({k: v})
                else:
                        valid.update({k: v})

        valid_cell = list(valid.keys())
        valid_hab = list(valid.values())


        occupied = np.zeros(23)
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

        print(h)
        print(a)
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
        print("Final Habitats list")
        print("-------------------")
        print(h)
        print("-------------------")
        print("Final Animals list")
        print("-------------------")
        print(a)
        final_animals = ", ".join(a)
        final_habitats = ", ".join(h)
        with open('final.txt', 'w') as f:
                for j in final_habitats:
                        f.write(j)
                f.write("\n")
                for i in final_animals:
                        f.write(i)

if __name__ == "__main__":
        dic = {'4': 'medium residential area', '1': 'basketball court', '15': 'snowberg', '11': 'medium residential area', '20': 'sparse residential area',
                '17': 'circular farm', '24': 'river', '23': 'sea glacier','22': 'sparse residential area', '21':'beach', 'F1': 'arctic fox',
                'E1': 'kangaroo', 'D1': 'cow', 'C1': 'snow leopard', 'F2': 'penguin', 'A2': 'lion', 'A3': 'arctic fox', 'F4': 'arctic fox',
                'A4': 'skunk', 'F5': 'seal', 'F6': 'lion','E6': 'bear', 'D6': 'snake', 'C6': 'bee', 'B6': 'zebra', 'A1': 'sheep'}
        do_mapping(dic)
        pass