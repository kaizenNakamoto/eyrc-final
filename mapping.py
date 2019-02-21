import pandas as pd

file = 'Animal_Habitat_Mapping.xlsx'
df = pd.read_excel(file, sheet_name=0,index_col=0)

#maps = {'C1' : 'dog', '17' : 'dense residential', 'D6' : 'skunk', 'A3' : 'zebra', '11' : 'wetland', '8' : 'mountain'}

maps = {'7': 'cloud', '6': 'river', '14': 'shrubs', '18': 'thermal power station', '17': 'snowberg', '21': 'beach', 'F1': 'bee', 'D1': 'penguin', 'C1': 'arctic fox', 'B1': 'hippo', 'F2': 'skunk', 'A2': 'fish', 'F3': 'fish', 'A3': 'cow', 'F4': 'cow', 'A4': 'frog', 'F5': 'deer', 'E6': 'deer', 'D6': 'eagle',
'A6': 'arctic fox'}

animal_loc = []
habitat_loc = []
animals = []
pref = []
habitats = []
animal_dict = {}
habitat_dict = {}

animal_labels = ['arctic fox', 'bear', 'bee', 'butterfly', 'cat', 'cougar', 'cow', 'coyote', 'crab',
        'crocodile', 'deer', 'dog', 'eagle', 'elephant', 'fish', 'frog', 'giraffe',
        'goat', 'hippo', 'horse', 'kangaroo', 'lion', 'monkey', 'otter', 'panda',
        'parrot', 'penguin', 'raccoon', 'rat', 'seal', 'shark', 'sheep', 'skunk',
        'snake', 'snow leopard', 'tiger', 'yak', 'zebra']

def do_map(maps):

    for key, value in maps.items():
        if key[0].isalpha():
            animal_dict.update({key:value})
        else:
            habitat_dict.update({key:value})

    for k,v in animal_dict.items():
        animal_loc.append(k)
        animals.append(v)


    for k,v in habitat_dict.items():
        habitat_loc.append(k)
        habitats.append(v)

    for idx,ani in enumerate(animals):
        count, locate = location(ani)
        if count == 1:
            pref.append(ani)
        hab = assign_cell(locate)
        print("%s(%s) - %s(%s)" %(ani,animal_loc[idx], locate[idx],hab))
    #print(count)


loc_str = ", ".join(animal_loc)
with open('map_1.txt', 'w') as f:
    for i in loc_str:
        f.write(i)

def location(animal):
    x = 0
    pos = []
    for i,_ in df.iterrows():
        for j in df.columns:
            if df[j][i] == animal:
                pos.append(i)
                x += 1
    return x, pos

def assign_cell(hloc):
    flag = 0
    for idex,h in enumerate(habitats):
        if hloc[idex] == h:
            flag = 1
            return habitat_loc[idex]
