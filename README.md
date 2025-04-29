# doom-paper

## What am I doing here?

### Get data

I've downloaded [a big WAD file repository](https://archive.org/details/idgames_202003) (~20GB) from the Internet Archive. They have [a bigger one](https://archive.org/details/wadarchive) (~975GB) but currently I do not have the space to process it, so let's work for a while with the "smaller" one until we get some results.

Uncompress the zip file (twice, it has another zip inside), so you will have a big collection of smaller zip files inside `/idgames_202003/idgame`.

Use the script in `scripts/unzip-everything.sh` to uncompress all the zip files. This will take a while, so be patient. After the script finishes, you will have a really big collection of directories (~41GB) with WAD and TXT files with descriptions and informations about such WAD files.

## Filter data

I've built the `waddup` command line tool in C++ to filter the WAD files. It will take a directory with a lot of WAD files and any other things and will create a new directory with only the non-repeated WAD files.

```bash
cd tools/waddup

# The tool is built with conan and make
conan profile detect
make

build/waddup ../../idgames_202003/idgames/ ../../wads
```

The tool will take a while to run, but it will create a new directory with the non-repeated WAD files. For the curious, the tool groups wad files by file size, and then by SHA256 hash. It will copy the first instance of each WAD file to the new directory.

The output will look like this:

```bash
$ build/waddup ../../idgames_202003/idgames/ ../../wads
WAD DUPlicate detector:
Searching for WAD files in directory: ../../idgames_202003/idgames/
Total WAD files found: 7614
Progress: 100.0%
Processing complete.

Found duplicate WAD files with hash 4eb361c32b6bf1db350488f592e65eeeea17f0821d2b26966bf92106e84ccdb7:
  ../../idgames_202003/idgames/combos/rdnd/rdndtest.wad (4294 bytes)
  ../../idgames_202003/idgames/combos/rdnd15/rdndtest.wad (4294 bytes)
Copied first instance to destination: ../../wads/007425_combos_rdnd_rdndtest.wad

...
```

With this filter we can move from a collection of ~41GB of mixed files to ~27GB of non repeated WADs.
