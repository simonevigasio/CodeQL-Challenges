import pandas as pd

db = pd.read_pickle('reveal_test.pkl')

for l in db['label']:
    file = open('labels', 'a')
    file.write(str(l)+'\n')

file.close()