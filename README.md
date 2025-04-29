# doom-paper

## What am I doing here?

### Get data

I've downloaded [a big WAD file repository](https://archive.org/details/idgames_202003) (~20GB) from the Internet Archive. They have [a bigger one](https://archive.org/details/wadarchive) (~975G) but currently I do not have the space to process it, so let's work for a while with the "smaller" one until we reach some results.

Uncompress the zip file (twice, it has another zip inside), so you will have a big collection of smaller zip files inside `/idgames_202003/idgame`.

Use the script in `scripts/unzip-everything.sh` to uncompress all the zip files. This will take a while, so be patient. After the script finishes, you will have a really big collection of directories with WAD and TXT files with descriptions and informations about such WAD files.
