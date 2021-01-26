# gsrsup

`gsrsup` implements _resumable streaming uploads_ to Google Cloud Storage. It’s
useful for uploading a reproducible stream of data too large to fit on disk. For
example, you can upload a ZFS snapshot by running

    zfs send tank/fs@snapshot | gsrsup gs://bucket/object

This pipeline can be interrupted at any time; if rerun, `gsrsup` will seek to the
appropriate place in the input stream and continue uploading from there.

`gsrsup` performs integrity checking on the data it uploads and will report a
problem if the input stream changes between two invocations. However, the design
of the Google Cloud Storage API prevents `gsrsup` from detecting such a change
until the entire stream has been uploaded.

For safety, `gsrsup` treats Google Cloud Storage objects as append-only – it
will never overwrite or delete them. Furthermore, it will never append to an
object once its upload has completed. If you run

    gsrsup gs://bucket/bash </bin/bash

twice, the second invocation will skip uploading altogether and only run the
integrity-checking code.

---

This is not an official Google product.
