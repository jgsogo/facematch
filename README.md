# Facematch

This is a toy of example using the OpenCV library in C++. The objective
of this project is just to play with some functions of the library
related to face recognition.

So... what does this do?

 * Identify faces in a dataset of photographs.
 * Train several classifiers
 * Look for best matchings using those classifiers.


# Example usage

In this section I will lead you through the code and a working example. We
are going to build two datasets of photographs, one will be labelled and
we will use it to train the classifiers that we are going to apply to
the other dataset afterwards.

## Download sources and compile

First of all, you need to download the sources and compile the C++ projects

```
git clone https://github.com/jgsogo/facematch
cd facematch
git submodule update --init
cmake . -Bbuild
cd build
make
```

## Build datasets

### Train dataset (politicians)
The train dataset we are going to use will be related to spanish politicians
as we are in an electoral period (there will be elections in Spain in
May 24th, 2015). So we need photos of the main candidates. There is a python
script ready to use:

```
cd facematch/utils
python download_training_images.py dataset/politicians
```

This script will search in Google images using the name of the leaders of the
six major political parties of Spain (inspect the to know their names), and
will store each set of images in a different folder (go to your `dataset/politicians`
and check it).

After downloading the images we need to create the CSV file we are going
to use as input for our program. There is another script prepared to this:

```
cd facematch/utils
python build_csv.py dataset/politicians working_dir/politicians
```

This script will create a file called `politicians.csv` in your `working_dir/politicians/`
directory where each row contains the `id`, `path` and `label` for each image. There must
be also a file called `labels.csv` with the readable name of each label.

In order to use this dataset we need to create another file, a config file
where we will point to the configurations and datasets to use during the execution
of our programs. So create file `working_dir/politicians/politicians.cfg`
with the following content:

```
faces_min_size=30
dataset=<path/to>/working_dir/politicians/politicians.csv
faces=<path/to>/working_dir/politicians/faces.csv
labels=<path/to>/working_dir/politicians/labels.csv
```

This dataset is ready to use.

### Work dataset (Flickr)

We need another dataset to classify. I've used photos from my friends stored in Flickr, 
there is a python script at `facematch/utils/down_flickr_album.py` you can use to
download full collections from Flickr, although you will need to create a secret file
with your credentials in order to access your content.

Any collection of photos with people in it will work. You will have to perform the same
steps shown for the previous dataset.

I will refer to this dataset as `friends`, and I will place it at `working_dir/friends/`.


## Identify faces
The next step is to identify the faces in both datasets. In this toy example there is a
program that will identify the faces and create a set of images with them. You can
execute it typing:

```
cd facematch/build
./faceclustering/faceclustering working_dir/politicians/politicians.cfg
```

This can take a while, it will create the images (you can find them in `working_dir/politicians/`)
and will perform to clustering techniques. You can explore the resulting files and find
the most similar images whithin the dataset.

Work also with your `friends` dataset:

```
cd facematch/build
./faceclustering/faceclustering working_dir/friends/friends.cfg
```

Now you have all the faces of your friends cropped at `working_dir/friends`.


## Classify
In order to perform the classification you have to call another program givin to it as
arguments which are the training and the work datasets. Use:

```
cd facematch/build
./facerecognition/facerecognition working_dir/politicians/politicians.cfg working_dir/friends/friends.cfg`
```

The output will show you who of your friends looks like which one of the leaders of the Spanish politics
environment (lol), and will give you an overview of the results of these particular elections.


Enjoy it!

