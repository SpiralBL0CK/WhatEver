import pandas as pd
import numpy as np
import warnings
import matplotlib.pyplot as plt
import seaborn as sns
%matplotlib inline
warnings.filterwarnings('ignore')

df = pd.read_csv('ratings.csv', sep='\t', names=['user_id','item_id','rating','titmestamp'])

movies_titles = pd.read_csv('movies.csv')

df = pd.merge(df, movie_titles, on='item_id')

df.head()

print df.head()
print '\n'
print movies_titles.head()

ratings = pd.DataFrame(df.groupby('title')['rating'].mean())

print ratings.head()

ratings['number_of_ratings'] = df.groupby('title')['rating'].count()
ratings.head()
ratings['rating'].hist(bins=50)
ratings['number_of_ratings'].hist(bins=60)

sns.jointplot(x='rating', y='number_of_ratings', data=ratings)

movie_matrix = df.pivot_table(index='user_id', columns='title', values='rating')
movie_matrix.head()

ratings.sort_values('number_of_ratings', ascending=False).head(10)