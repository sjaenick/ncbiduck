Usage:

I. Initializing a new database

createdb initializes a new database; for this, NCBI taxonomy data and
a scoring method is needed. All taxonomy nodes are initialized with the
maximum possible score. By default, a mean score is used, but this can
be adapted to use the geometric mean by suppying the -g parameter.

createdb -t /vol/biodb/local_databases/MGX/ncbi_taxonomy -d /path/to/database [ -g ]

II. Importing ReferenceSeeker results

rsimport imports ReferenceSeeker results into an existing database, updating
the stored score information as needed.

rsimport -d /path/to/database -r /path/to/data.tsv

data.tsv is a tab-separated file, consisting of two columns:
<correct assembly tax ID>\t<path to referenceseeker output>
