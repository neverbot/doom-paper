# doom-paper

## What am I doing here?

## Project Structure

This repository uses Git submodules for some tools:

- [tools/wadconvert](https://github.com/neverbot/wadconvert) - Tool to convert WAD files to JSON and other formats.
- [tools/waddup](https://github.com/neverbot/waddup) - Tool to detect and filter duplicate WAD files.

```bash
# Clone the repository with submodules
git clone --recursive git@github.com:neverbot/doom-paper.git
cd doom-paper
```

## Steps followed

### Get data

I've downloaded [a big WAD file repository](https://archive.org/details/idgames_202003) (~20GB) from the Internet Archive. They have [a bigger one](https://archive.org/details/wadarchive) (~975GB) but currently I do not have the space to process it, so let's work for a while with the "smaller" one until we get some results.

Uncompress the zip file (twice, it has another zip inside), so you will have a big collection of smaller zip files inside `/idgames_202003/idgame`.

Use the script in `scripts/unzip-everything.sh` to uncompress all the zip files. This will take a while, so be patient. After the script finishes, you will have a really big collection of directories (~41GB) with WAD and TXT files with descriptions and informations about such WAD files.

```bash
cd scripts
./unzip-everything.sh
```

### Filter data

I've built the [`waddup` command line tool](https://github.com/neverbot/waddup) in C++ to filter the WAD files. It will take a directory with a lot of WAD files and any other things and will create a new directory with only the non-repeated WAD files.

```bash
cd tools/waddup

# The tool is built with conan and make
conan profile detect
make

build/waddup ../../idgames_202003/idgames/ ../../wads
```

The tool will take just some seconds to run, and it will create a new directory with the non-repeated WAD files. For the curious, the tool groups wad files by file size, and then by SHA256 hash. It will copy the first instance of each WAD file to the new directory.

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

### Convert data

Now we can convert the WAD files to JSON format. I've built the C++ [`wadconvert` command line tool](https://github.com/neverbot/wadconvert) that will take a WAD file and convert it to a JSON file. The tool is built with conan and make, so you need to build it first.

```bash
cd tools/wadconvert

# The tool is built with conan and make
conan profile detect
make

# example of use
build/wadconvert ../../wads/000001_deathmatch_deathtag_behetag_Behetag.wad ../../test.json
```

Use the script in `scripts/wads2json.sh` to convert all the WAD files in the `wads` directory to JSON format. The script will use the `wadconvert` tool with every WAD file in the `wads` directory and will store the JSON files in the same directory. It will take a while to run, so be patient. After the script finishes, you will have a new directory with all the WAD files converted to JSON format.

```bash
cd scripts
./wads2json.sh
```

As the script could take a long time, it's made to run alphabetically, so you can stop it where you want (`ctrl + c`) and run it again later, starting where you want. If you provide the argument `--start`, it will skip files until it finds one that matched the wildcards provided. For example, if you converted the files until one that stars with `000123_`, you can run the script again with the argument `--start=000123*`.

```bash
$ cd scripts
$ ./wads2json.sh --start=000008*

Skipping: ../wads/000001_deathmatch_deathtag_behetag_Behetag.wad
Skipping: ../wads/000002_deathmatch_doombot_dbot51_ctflevel.wad
Skipping: ../wads/000003_deathmatch_doombot_dbot51_ZDoomB.wad
Skipping: ../wads/000004_deathmatch_facility_Facility.wad
Skipping: ../wads/000005_deathmatch_skulltag_99coop_99coop.wad
Skipping: ../wads/000006_deathmatch_skulltag_mek-ttge_Mek-TTGE.wad
Skipping: ../wads/000007_deathmatch_skulltag_basebldm_BASEBALLDM.wad
000008_deathmatch_skulltag_td2_td2.wad converted to JSON.
000009_deathmatch_skulltag_td5_TD5.wad converted to JSON.
...
```

