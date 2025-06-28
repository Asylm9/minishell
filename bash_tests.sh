
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

#############################################

attention '\n' parasite -> 18: ast->right->cmd->redirections->target = 0x555555596f80 "out.txt\n"

#############################################

echo meow > file.txt | ls -la | grep .h > out.txt | < file.txt cat

si ajout d'input ou cat avant file.txt :
==36124== Conditional jump or move depends on uninitialised value(s)
==36124==    at 0x10B86E: free_ast (free.c:80)
==36124==    by 0x10B862: free_ast (free.c:79)
==36124==    by 0x10B87F: free_ast (free.c:81)
==36124==    by 0x10B87F: free_ast (free.c:81)
==36124==    by 0x1097AC: main (main.c:89)
==36124==  Uninitialised value was created by a heap allocation
==36124==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==36124==    by 0x10B06D: parse_ast (parser.c:144)
==36124==    by 0x109789: main (main.c:87)

fonctionne correctement lorsqu'on s'arrete a out.txt.

#############################################

WTF:
Test 321/174: ❌ $HOMEdskjhfkdshfsd' 
mini exit code = 1
bash exit code = 2
mini error = ()
bash error = ( unexpected EOF while looking for matching `'')


#############################################


