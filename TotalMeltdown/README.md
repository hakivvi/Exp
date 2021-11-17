# TotalMeltdown:
Exploit of CVE-2018-1038

# short wu:
the self ref entry in the PML4 of Windows 7 (update JAN 2018) was accessible by the usermode (U/S flag set). the exploit uses the self ref entry to add another PML4 entry in the upper half of the PML4 (the kernel half), once we add our PML4E we map ~30GB of physical memory using 31 PDP each of them maps 512 huge page (2MiB page).  
Once we map the memory, we use some members of _EPROCESS structure to idenify the System process and the parent process, and finally duplicate the Token.

# PoC

![PoC](TotalMeltdown.gif)
the exploit in the PoC found the structures in the memory fastly because i narrowed down the search region, but searching the whole memory will take sometime.
