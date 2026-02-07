# Parrot

parrot is a command line utility that automatically duplicates a processes stdout and stderr to files. To run a process with parrot simply include parrot ahed of your usual terminal command. For example, to use parrot with the `ls` utility, run:

```bash
parrot ls
```

This will automatically write all stdout and stderr to "stdout.log" and "stderr.log" respectively.

## Parrot Options

You can optionally change the files that stdout and stderr are duplicated to. Use the command line option "-o" with an argument to specify a different file for stdout. Use the command line option "-e" with an argument to specify a different file for stderr. 

_Note: When using parrot command line options a double dash must be used to separate the options from the process you wish to run._

For example, to run `ls` but route stdout to "output.log" run the command:

```bash
parrot -o output.log -- ls
```
