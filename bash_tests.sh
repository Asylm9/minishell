
# arg de cmd peut se trouver apres redir 
bash-5.1$ echo > infile1 "BLBLBL"
bash-5.1$ cat infile1 
BLBLBL

bash-5.1$ echo < infile1 "lol"
lol 
bash-5.1$ cat infile1 
BLBLBL 

bash-5.1$ cat infile.txt
test
bash-5.1$ < infile.txt grep t
test
bash-5.1$ grep t < infile.txt
test

echo pif > output.txt < output.txt cat
grep "pif" < output.txt > matches.txt

