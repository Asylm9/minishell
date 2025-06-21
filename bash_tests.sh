
# arg de cmd peut se trouver apres redir 
bash-5.1$ echo > infile1 "BLBLBL"
bash-5.1$ cat infile1 
BLBLBL

bash-5.1$ echo < infile1 "lol"
lol 
bash-5.1$ cat infile1 
BLBLBL 


