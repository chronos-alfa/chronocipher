# Chronocipher

A simple 2048b[^bits] symmetric cipher.

## Build

`Make` is used to build the application. Command:

```sh
make
```

will build the library objects and run the tests.

To build the actual cli application, run command:

```sh
make build 
```

## Program usage:

If you run the application `chronocipher` without any parameters, you will be present with the following usage output:

```
USAGE:
chronocipher <option> <filepath>

ARGUMENTS
filepath  Path to the file

OPTIONS
-gr  Generate random key to the <filepath>
-gp PASSPHRASE  Generate random key to the <filepath> from a passphrase
-e KEYFILE  Encrypt the file in the <filepath> with key from the KEYFILE
-d KEYFILE  Decrypt the file in the <filepath> with key from the KEYFILE
```

### Usage examples

```sh
chronocipher -gr my.key
```

This command will generate the key.

```sh
chronocipher -gp mypassphrase my.key
```

This command will generate the key from the passphrase. This is less secure, but the generated key will always by the same for the same passphrase.

```sh
chronocipher -e my.key myfile.txt
```

This will encrypt the file `myfile.txt` with the key from `my.key` file.

**Note:** The original file doesn't get rewritten, instead the suffix *_enc* is added to the existing filename, so the encrypted file in our case will reside in `myfile.txt_enc`.

```sh
chronocipher -d my.key myfile.txt
```

This will decrypt the file `myfile.txt` and store the decrypted version in the file `myfile.txt_dec` as the suffix *_dec* is used for decryption.

[^bits]: The key is made of 256 unique bytes, giving the actual complexity of `256!`.
