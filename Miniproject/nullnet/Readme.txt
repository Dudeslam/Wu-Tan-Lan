Vi sender kun data videre fra node 2, hvis den modtager fra node 4. Data fra node 3 bliver gemt indtil node 4 sender.

Har problem med at få nulstillet dataen, så node 2 sender mere end den burde efter første besked. Som jeg lige ser det.
En ide kunne være som skrvet nedenfor.
I linje 163 i uni 2 koden, er der en potenitial løsning. Så skal der bare appendes modsat tænker jeg, men har ikke testet det endnu desværre.

Den overordnede mangel er at dataen skal pakkes ud korrekt i sink, har ikke fået lavet det med det ekstra data vi sender. Og så skal aggregeringen forgå korrekt, altså med at bitshifte bytes ind i vores count som er af størrelsen 8 byte (unsigned long long int). Her sker der også en fejl, ser ud til at den huske noget data som skrevet ovenfor.

Har ikke fået set på energi målinger, med energest. Har skrevet til lukas angående det, men i må gerne se på det. 

Har tilføjet et lille billede med et look up table - rough sketch bare af de bytes vi sender og hvad de svarer til. 

