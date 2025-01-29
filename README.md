
# Miris

## Description

Developed a Linux-based C application for managing and analyzing financial transactions between users using a dynamic directed multi-graph structure. The program supports efficient node and edge operations, cycle detection, connectivity analysis, and memory management. Implemented a custom hash table for O(1) access and ensured modularity with separate compilation and a Makefile.

## To Do

- [Change hash table size to be the closest prime number](https://github.com/iliaschr/OSProject1/issues/1)  
  _Adjust the hash table size to improve performance by setting it to the nearest prime number._

- [Add pointer to list of incoming edges in node struct](https://github.com/iliaschr/OSProject1/issues/2)  
  _Introduce a pointer in the `node` struct for a list of incoming edges to optimize edge traversal complexity._

- [Adjust hash table allocation to handle multiple nodes per line](https://github.com/iliaschr/OSProject1/issues/3)  
  _Update hash table memory allocation to account for lines that introduce multiple nodes._

## Γρήγορες σημειώσεις για το Makefile

Για να χρησιμοποιήσεις το Makefile αυτού του έργου, ακολούθησε τα παρακάτω βήματα:

1. **Μετάβαση στον φάκελο src**
   ```sh
   cd src/

2. **Εκτέλεση των διαθέσιμων εντολών με το make**:

- **`make all`**: Δημιουργεί το κύριο εκτελέσιμο πρόγραμμα καθώς και τα εκτελέσιμα αρχεία για τις δοκιμές (`graph_test_exec` και `miris_test_exec`).
- **`make build_miris`**: Δημιουργεί και εκτελεί το κύριο πρόγραμμα `miris`. Χρησιμοποιείται για την εκτέλεση της βασικής εφαρμογής.
- **`make run`**: Εκτελεί το κύριο πρόγραμμα `miris` (με προϋπόθεση ότι έχει ήδη δημιουργηθεί).
- **`make graph_test`**: Δημιουργεί και εκτελεί τις δοκιμές για τη μονάδα `graph`.
- **`make miris_test`**: Δημιουργεί και εκτελεί τις δοκιμές για τη μονάδα `miris`.
- **`make valgrind_graph_test`**: Εκτελεί τις δοκιμές για το `graph` με το Valgrind για την ανίχνευση διαρροών μνήμης.
- **`make valgrind_miris_test`**: Εκτελεί τις δοκιμές για το `miris` με το Valgrind για τον ίδιο σκοπό.
- **`make valgrind_test`**: Εκτελεί και τις δύο δοκιμές (`graph` και `miris`) με Valgrind για πλήρη έλεγχο διαρροών μνήμης.
- **`make clean`**: Καθαρίζει όλα τα παραγόμενα αρχεία (.o και εκτελέσιμα) και αρχεία εξόδου (`test_output.txt`, `output.txt`).


## Graph.h

Ξεκίνησα την εργασία δημιουργώντας μια γενική δομή graph. Μέσα σε αυτή με βάση το [Lecture Code από τις Δομές Δεδομένων](https://github.com/chatziko-k08/lecture-code/tree/master) έφτιαξα μερικά functions για να με βοηθήσουν με την διαχείριση του γράφου.

### Σχετικά με το Graph.h

Στο graph.h, δηλώνονται οι δομές δεδομένων και οι συναρτήσεις που σχετίζονται με την διαχείριση του γράφου:

- Edge: Μια ακμή του γράφου, περιλαμβάνει πληροφορίες για τον προορισμό, το ποσό και την ημερομηνία.
- Node: Ένας κόμβος, περιλαμβάνει πληροφορίες σχετικά με το id (μοναδικό αναγνωριστικό του), τις ακμές που συνδέονται με αυτόν και την ένδειξη αν έχει επισκεφτεί από κάποιο dfs function (προστέθηκε μετά όταν έφτιαξα τα αρχεία miris).
- Graph: Περιλαμβάνει ένα πίνακα κατακερματισμού για την αποθήκευση των κόμβων.

## Graph.c

Όλες οι υλοποιήσεις των λειτουργιών για την διαχείριση των graph.

### Σχετικά με το Graph.c

1. **graph_create(Δημιουργία γράφου)**:
    - Δημιουργεί έναν νέο γράφο με μέγεθος __initial_size__ (Προστέθηκε μετά για να διευκολύνει την δημιουργία γράφων από αρχείο .txt)
    - Χρησιμοποιεί __malloc__ για να δεσμεύσει μνήμη για τον γράφο και __calloc__ για τον πίνακα κατακερματισμού, διασφαλίζοντας ότι οι αρχικές τιμές είναι __NULL__.
    - Πολυπλοκότητα __O(1) για την προσπέλαση__ και __O(n) για την αρχικοποίηση__ (n το μέγεθος του πίνακα).

2. **node_insert(Εισαγωγή Κόμβου)**:
    - Ελέγχει αν υπάρχει ήδη ο κόμβος.
    - Πολυπλοκότητα __O(1) για την εύρεση__ και __O(1) για την δημιουργία του κόμβου__.

3. **node_find(Εύρεση κόμβου)**:
    - Επιστρέφει τον κόμβο με το συγκεκριμένο id αν υπάρχει.
    - Χρησιμοποιεί την find_node_in_hash_table για να εντοπίσει τον κόμβο.
    - Πολυπλοκότητα __O(1) για την αρχική πρόσβαση__ και __O(n) για την περιήγηση της λίστας__.

4. **node_remove(Διαγραφή Κόμβου)**:
    - Αφαιρεί τον κόμβο και τις ακμές που συνδέονται με αυτόν.
    - Ελέγχει πρώτα αν υπάρχουν ακμές προς τον κόμβο που διαγράφεται.
    - Πολυπλοκότητα __O(n) για την αναζήτηση edges__ και __O(1) για την αφαίρεση__.

5. **edge_insert (Εισαγωγή Ακμής)**:
    - Δημιουργεί μια νέα ακμή μεταξύ δύο κόμβων.
    - Ελέγχει την ύπαρξη των κόμβων πηγής και προορισμού πριν την εισαγωγή.
    - Πολυπλοκότητα __O(1) για την εισαγωγή αν υπάρχουν ήδη οι κόμβοι__.

6. **edge_remove (Διαγραφή Ακμής)**:
    - Αφαιρεί μια συγκεκριμένη ακμή μεταξύ δύο κόμβων.
    - Πολυπλοκότητα __O(n) για την αναζήτηση της ακμής__.

7. **edge_modify (Τροποποίηση Ακμής)**:
    - Τροποποιεί την ποσότητα και την ημερομηνία μιας ακμής, ελέγχοντας αν οι κόμβοι και η ακμή υπάρχουν.
    - Πολυπλοκότητα __O(n) για την αναζήτηση της ακμής__.

8. **graph_destroy (Καταστροφή Γράφου)**:
    - Απελευθερώνει όλη τη μνήμη που έχει δεσμευτεί για τον γράφο και τους κόμβους του.
    - Πολυπλοκότητα __O(n) για την απελευθέρωση των κόμβων και των ακμών__.

#### Static functions
Αν και θα μπορούσαν να μην υπάρχουν, αναγκάστηκα να τα βάλω καθώς βοηθάνε στο να κάνουν πιο ευανάγνωστα μερικά functions.

1. **find_node_in_hash_table**:
    - Εδώ πρέπει να γίνει μια παρατήρηση σχετικά με την πολυπλοκότητα γενικά αφού κάνω malloc με initial_size στην graph η πιθανότητα να μην υπάρχει χώρος για κόμβους θα είναι μικρή, άρα στην ιδανική περίπτωση η πολυπλοκότητα είναι O(1).

2. **remove_edges_to_node**:
    - Αφαιρεί όλες τις ακμές που δείχνουν προς τον κόμβο.
    - Χρησιμοποιεί την edge_remove.
    - Πολυπλοκότητα __O(n)__ καθώς μπορεί να χρειαστεί να περιηγηθεί σε όλους τους κόμβους.

3. **free_node_edges**:
    - Ελευθερώνει όλη τη μνήμη που έχει δεσμευτεί για τις ακμές ενός συγκεκριμένου κόμβου.
    - Πολυπλοκότητα __O(m)__, όπου m ο αριθμός των ακμών.

4. **remove_node_from_hash_table**:
    - Απλή ενημέρωση των δεικτών άρα __O(1)__.

5. **create_edge**: 
    - Δημιουργεί και επιστρέφει μια νέα ακμή.
    - Πολυπλοκότητα __Ο(1)__.

6. **find_edge**:
    - Εντοπίζει μια ακμή που συνδέει δύο κόμβους.
    - Πολυπλοκότητα __O(m)__.

7. **remove_edge_from_list**:
    - Πολυπλοκότητα __O(m)__.

## Miris.h και Miris.c

Δημιουργήθηκαν μετά τα graph.h και graph.c αρχεία για να βοηθήσει στην δημιουργία των functions που χρειάζονται για την εργασία.

### Σχετικά με το Miris.c

1. **miris_insert (Εισαγωγή κόμβου)**:
   - Δημιουργεί έναν νέο κόμβο στον γράφο. Εάν ο κόμβος υπάρχει ήδη, επιστρέφει αμέσως.
   - Χρησιμοποιεί τη συνάρτηση `node_insert` για να διαχειριστεί την προσθήκη.
   - Πολυπλοκότητα: __O(1)__ κατά μέσο όρο, εφόσον η κατανομή του hash table είναι καλή.

2. **miris_insert2 (Εισαγωγή ακμής)**:
   - Δημιουργεί νέους κόμβους αν δεν υπάρχουν ήδη οι κόμβοι πηγής και προορισμού.
   - Χρησιμοποιεί τη `edge_insert` για να προσθέσει την ακμή μεταξύ των δύο κόμβων.
   - Πολυπλοκότητα: __O(1)__ για την εύρεση των κόμβων, __O(1)__ για την προσθήκη της ακμής αν δεν υπάρχουν συγκρούσεις.

3. **miris_delete (Διαγραφή κόμβου)**:
   - Αφαιρεί έναν κόμβο από τον γράφο, καθώς και όλες τις ακμές που σχετίζονται με αυτόν.
   - Ελέγχει πρώτα αν ο κόμβος υπάρχει.
   - Πολυπλοκότητα: __O(n)__ για την αφαίρεση, όπου n είναι ο αριθμός των κόμβων που συνδέονται με τον συγκεκριμένο κόμβο.

4. **miris_delete2 (Διαγραφή ακμής)**:
   - Αφαιρεί μια συγκεκριμένη ακμή μεταξύ δύο κόμβων.
   - Ελέγχει αν οι κόμβοι πηγής και προορισμού υπάρχουν.
   - Πολυπλοκότητα: __O(n)__ στον χειρότερο των περιπτώσεων για την εύρεση της ακμής.

5. **miris_modify (Τροποποίηση ακμής)**:
   - Τροποποιεί την ποσότητα και την ημερομηνία μιας ακμής αν βρεθεί.
   - Ελέγχει αν οι κόμβοι πηγής και προορισμού υπάρχουν.
   - Πολυπλοκότητα: __O(n)__ για την αναζήτηση της ακμής.

6. **miris_find (Εύρεση κόμβου)**:
   - Εμφανίζει όλες τις εξωτερικές ακμές που συνδέονται με έναν συγκεκριμένο κόμβο.
   - Χρησιμοποιεί τη `node_find` για να βρει τον κόμβο.
   - Πολυπλοκότητα: __O(m)__, όπου m είναι ο αριθμός των ακμών του κόμβου.

7. **miris_receiving (Εύρεση εισερχόμενων ακμών)**:
   - Εμφανίζει όλες τις ακμές που οδηγούν στον κόμβο.
   - Διερευνά ολόκληρο τον γράφο για να βρει ακμές που οδηγούν στο συγκεκριμένο κόμβο.
   - Πολυπλοκότητα: __O(n*m)__, όπου n είναι ο αριθμός των κόμβων και m οι ακμές.

8. **miris_circlefind (Εύρεση κύκλων)**:
   - Χρησιμοποιεί DFS για να ανακαλύψει κύκλους στον γράφο που ξεκινούν από έναν συγκεκριμένο κόμβο.
   - Χρησιμοποιεί έναν πίνακα για την παρακολούθηση των κόμβων που έχουν επισκεφθεί.
   - Πολυπλοκότητα: __O(n + m)__, όπου n είναι οι κόμβοι και m είναι οι ακμές.

9. **miris_findcircles (Εύρεση κύκλων με ελάχιστο ποσό)**:
   - Εφαρμόζει την DFS για να βρει κύκλους με βάση μια ελάχιστη τιμή ποσού για τις ακμές.
   - Πολυπλοκότητα: __O(n + m)__, που μπορεί να επηρεαστεί από τον αριθμό των ακμών.

10. **miris_traceflow (Ιχνηλάτηση ροής)**:
    - Διεξάγει DFS για να παρακολουθήσει τη ροή των συναλλαγών από έναν κόμβο.
    - Πολυπλοκότητα: __O(n + m)__ για την πλήρη διαδρομή των συναλλαγών.

11. **miris_connected (Ελέγχει αν υπάρχει σύνδεση)**:
    - Ελέγχει αν υπάρχει διαδρομή μεταξύ δύο κόμβων.
    - Πολυπλοκότητα: __O(n + m)__ λόγω του DFS.

12. **miris_exit (Έξοδος από το πρόγραμμα)**:
    - Καταγράφει την κατάσταση του γράφου σε αρχείο εξόδου.
    - Ελευθερώνει τη μνήμη των κόμβων και των ακμών.
    - Πολυπλοκότητα: __O(n + m)__ λόγω της απελευθέρωσης μνήμης για τους κόμβους και τις ακμές.

### Σχετικά με τις Static Συναρτήσεις

1. **dfs_find_cycles**:
   - Εκτελεί Depth-First Search (DFS) για να βρει κύκλους που ξεκινούν από έναν συγκεκριμένο κόμβο.
   - Χρησιμοποιεί αναδρομή και μια δομή path για να παρακολουθήσει τους κόμβους που έχουν επισκεφθεί.
   - Πολυπλοκότητα: __O(n + m)__.

2. **dfs_find_circles_with_min_sum**:
   - Αναζητά κύκλους που πληρούν μια ελάχιστη τιμή ποσού για τις ακμές.
   - Χρησιμοποιεί παρόμοια λογική με την `dfs_find_cycles`.
   - Πολυπλοκότητα:__O(n + m)__.

3. **dfs_traceflow**:
   - Ιχνηλατεί τη ροή των συναλλαγών από τον τρέχοντα κόμβο, εκτυπώνοντας τους κόμβους της διαδρομής.
   - Πολυπλοκότητα: __O(n + m)__.

4. **dfs_find_path**:
   - Βρίσκει τη διαδρομή από τον τρέχοντα κόμβο προς τον προορισμό.
   - Πολυπλοκότητα: __O(n + m)__.


# Main aka. Miris the main program

### Σχετικά με το Main.c

1. **load_graph (Φόρτωση γράφου από αρχείο)**:
   - Διαβάζει δεδομένα από ένα αρχείο και εισάγει κόμβους και ακμές στον γράφο.
   - Χρησιμοποιεί `fscanf` για να διαβάσει τις τιμές και `miris_insert` και `miris_insert2` για να προσθέσει τους κόμβους και τις ακμές.
   - Πολυπλοκότητα: O(n), όπου n είναι ο αριθμός των γραμμών στο αρχείο.

2. **count_lines (Καταμέτρηση γραμμών)**:
   - Επιστρέφει τον αριθμό των γραμμών σε ένα αρχείο για να καθορίσει το μέγεθος του γράφου.
   - Χρησιμοποιεί `fgetc` για να διαβάσει το αρχείο χαρακτήρα προς χαρακτήρα.
   - Πολυπλοκότητα: O(n), όπου n είναι ο αριθμός των χαρακτήρων στο αρχείο.

3. **handle_insert (Διαχείριση εντολής εισαγωγής κόμβων)**:
   - Χειρίζεται την εισαγωγή πολλών κόμβων.
   - Χρησιμοποιεί `miris_insert` για να προσθέσει κάθε κόμβο.
   - Πολυπλοκότητα: O(m), όπου m είναι ο αριθμός των κόμβων που εισάγονται.

4. **handle_insert2 (Διαχείριση εντολής εισαγωγής ακμών)**:
   - Χειρίζεται την εισαγωγή μιας ακμής μεταξύ δύο κόμβων.
   - Χρησιμοποιεί `miris_insert2` για να προσθέσει την ακμή.
   - Πολυπλοκότητα: O(1) αν δεν υπάρχουν συγκρούσεις.

5. **handle_delete (Διαχείριση εντολής διαγραφής κόμβων)**:
   - Χειρίζεται τη διαγραφή πολλών κόμβων.
   - Χρησιμοποιεί `miris_delete` για να διαγράψει κάθε κόμβο.
   - Πολυπλοκότητα: O(m), όπου m είναι ο αριθμός των κόμβων που διαγράφονται.

6. **handle_delete2 (Διαχείριση εντολής διαγραφής ακμών)**:
   - Χειρίζεται τη διαγραφή μιας συγκεκριμένης ακμής.
   - Χρησιμοποιεί `miris_delete2` για να διαγράψει την ακμή.
   - Πολυπλοκότητα: O(1) αν η ακμή υπάρχει.

7. **handle_modify (Διαχείριση εντολής τροποποίησης ακμών)**:
   - Χειρίζεται την τροποποίηση μιας ακμής.
   - Χρησιμοποιεί `miris_modify` για να τροποποιήσει την ακμή.
   - Πολυπλοκότητα: O(n) για την αναζήτηση της ακμής.

8. **execute_command (Εκτέλεση εντολών)**:
   - Χειρίζεται την εκτέλεση των εντολών που εισάγει ο χρήστης.
   - Χρησιμοποιεί τις προηγούμενες συναρτήσεις χειρισμού για να εκτελέσει την κατάλληλη εντολή.
   - Πολυπλοκότητα: Εξαρτάται από την εντολή που εκτελείται.

9. **main (Κύρια λειτουργία)**:
   - Διαχειρίζεται τις παραμέτρους εισόδου και εκκινεί το πρόγραμμα.
   - Φορτώνει τον γράφο από ένα αρχείο αν παρέχεται.
   - Εμφανίζει ένα prompt για να δέχεται εντολές από τον χρήστη.
   - Πολυπλοκότητα: O(n) για τη φόρτωση του γράφου.

### Σχετικά με τις Static Συναρτήσεις

1. **handle_insert**:
   - Χειρίζεται την εισαγωγή κόμβων από τις εντολές του χρήστη.

2. **handle_insert2**:
   - Χειρίζεται την εισαγωγή ακμών από τις εντολές του χρήστη.

3. **handle_delete**:
   - Χειρίζεται τη διαγραφή κόμβων από τις εντολές του χρήστη.

4. **handle_delete2**:
   - Χειρίζεται τη διαγραφή ακμών από τις εντολές του χρήστη.

5. **handle_modify**:
   - Χειρίζεται την τροποποίηση ακμών από τις εντολές του χρήστη.


## Tests

Χρησιμοποίησα το `acutest.h` που είχαμε μάθει στις Δομές Δεδομένων. Τα test είναι αρκετά γενικά και γράφτηκαν όσο έγραφα τον κώδικα για να σιγουρευτώ ότι όλα πάνε καλά.

## Debugging

Όταν συναντούσα segmentation fault (core dumped) έκανα Debugging με gdb.

Επίσης, χρησιμοποίησα και valgrind για να αποφύγω τα leaks.

## Format

Το format έγινε για κάθε αρχείο με την εντολή `clang-format -i ...`.

## Specification for miris command generated-output:
In case of unrecognized command:
    ERROR INDICATION: "Unrecognized command"

0. COMMAND: ANY
    ERROR INDICATION: "Format error: " COMMAND_NAME COMMAND OPTIONS

1.  COMMAND: i Ni [Nj Nk ...]
    	 OR
    	 insert Ni [Nj Nk ...]
    COMMENT: insert into the graph structure 1 or more nodes
             with specific INT ids.
    EXPECTED OUTPUT:
    	 "Succ:" Ni Nj ..
    ERROR INDICATION:
    	 "IssueWith:" Nj Nk Nl ..

2.  COMMAND: n Ni Nj sum date
             OR
             insert2 Ni Nj sum date
    COMMENT: introduce an edge with direction from Ni to Nj with label
             sum + date if either Ni or Nj does not exist in the graph,
             do the appropriate node insertion first.
    EXPECTED OUTPUT:
    ERROR INDICATION: "IssueWith:" Ni Nj

3.  COMMAND: d Ni [Nj Nk ...]
             OR
             delete Ni [Nj Nk ...]
    COMMENT: delete from graph listed nodes Ni, Nj, Nk, etc
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni Nk ..

4.  COMMAND: l Ni Nj
             OR
             delete2 Ni Nj
    COMMENT: remove the edge between Ni and Nj; if there are
	     more than one edges, remove one of the edges.
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni ..

5.  COMMAND: m Ni Nj sum sum1 date date1
             OR
             m(odify) Ni Nj sum sum1 date date1
    COMMENT: update the values of a specific edge between Ni and Nj
    EXPECTED OUTPUT:
    ERROR INDICATION:
             "Non-existing node(s):" Ni ..
             OR
             "Non-existing edge:" Ni Nj sum date"

6.  COMMAND: f Ni
             OR
             f(ind) Ni
    COMMENT: find all outgoing edges from Ni
    EXPECTED OUTPUT:
             Ni Nj sum1 date1
             Ni Nk sum2 date2
             Ni Nl sum3 date3
             ...
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

7.  COMMAND: r Ni
             OR
             receiving Ni
    COMMENT: find all incoming to Ni edges.
    EXPECTED OUTPUT:
             Nj Ni sum1 data1
             Nl Ni sum2 data2
             Nl Ni sum3 data3
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

8.  COMMAND: c Ni
             OR
             circlefind Ni
    COMMENT: find circles Ni is involved in if any
    EXPECTED OUTPUT:
             Ni, Nj, Nk, .., Ni
             Ni, Na, Nb, Nc, ...,Ni
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni

9.  COMMAND: fi Ni k
             OR
             findcircles Ni k
    COMMENT: find circular relationships in which Ni is involved and
             moves at least k units of funds.
    EXPECTED OUTPUT:
             Ni, Nj, Nk, .., Ni m      // m min value found (m>=k)
             Ni, Na, Nb, Nc, ...,Ni d  // d min value found (d>=k)
             Ni, Nv, Ni o              // o min value found (o>=k)
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
            "Non-existing node:" Ni

10. COMMAND: t Ni m
             OR
             traceflow Ni m
    COMMENT: find all paths of length upto m for which a flow of transfers can
             be establsihed from Ni
    EXPECTED OUTPUT:
             Ni, Nj, Nl, Ny
             Ni, Nj, Nv
             Ni, Nj, Nv
             Ni, Na, Nc, Nb, Nx, Ny
             ....
             OR
             (none) // print nothing
    ERROR INDICATION:
            "Non-existing node:" Ni

11. COMMAND: o Ni Nj
             OR
             connected Ni Nj
    COMMENT: If exist, present a path (1) between Ni and Nj
    EXPECTED OUTPUT:
             Ni, No, .., Nm, Nj // connecting path
             OR
             (none) // print nothing
    ERROR INDICATION:
             "Non-existing node:" Ni
             "Non-existing node:" Nj

12. COMMAND: e
             OR
             exit
    COMMENT: terminate the program.
    EXPECTED OUTPUT:
             XYZ "Bytes released"
    ERROR INDICATION:
             (none)
