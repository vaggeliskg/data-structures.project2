![run-tests](../../workflows/run-tests/badge.svg)

## 2022 Project 2

Εκφώνηση: https://k08.chatzi.org/projects/project2/


### Προσωπικά στοιχεία

__Όνομα__: ΚΑΛΑΜΠΟΚΗΣ ΕΥΑΓΓΕΛΟΣ

__Α.Μ.__: sdi2100045

### Ασκήσεις που παραδίδονται

Συμπληρώστε εδώ τις ασκήσεις (και τυχόν bonus) που παραδίδετε. Αν κάποια άσκηση
βρίσκεται σε μη αναμενόμενα αρχεία αναφέρετε και τα αρχεία αυτά.

- ΑΣΚΗΣΗ 1:
    πολυπλοκότητες: queue_create -> O(1)
                    queue_size -> O(1)
                    queue_front -> O(1)
                    queue_back -> O(1)
                    queue_insert_back -> O(1)
                    queue_remove_front -> O(n)
                    queue_set_destroy_value -> O(1)
                    queue_destroy -> O(1)
- ΑΣΚΗΣΗ 2:
    πολυπλοκότητες: queue_create -> O(1)
                    queue_size -> O(1)
                    queue_front -> O(1)
                    queue_back -> O(1)
                    queue_insert_back -> O(1)
                    queue_remove_front -> O(1) (συνήθως) *
                    queue_set_destroy_value -> O(1)
                    queue_destroy -> O(1)

* queue_remove_front πλέον κάνει τις διαγραφές τις κατά κύριο λόγο κατευθείαν αφαιρώντας το τοπ στοχείο της 2ης στοίβας. Ο(n) θα είναι μόνο όταν αδειάζει η 2η στοίβα(σπάνια) και θέλω να κάνω διαγραφή επομένως μεταφέρω όλα τα στοιχεία της 1ης στη 2η.

-ΑΣΚΗΣΗ 3:

(περιέχεται και φάκελος graphs που περιέχει τα διαγράμματα που παραδίδονται)


 amortized list: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/list(amortized).png

amortized χρόνος με λίστα O(1) αφού όλες οι λειτουργίες της είναι Ο(1)


real list: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/list(real).png

real χρόνος με λίστα O(1) αφού όλες οι λειτουργίες της είναι Ο(1)

amortized stack: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/stack(amortized).png


amortized time με στοίβα γραμμικός αφού σε κάθε διαγραφή μεταφέρονται όλα στο vector και μετά πίσω πάλι


real stack: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/stack(real).png

real time με στοίβα O(n) αφού σε κάθε διαγραφή γίνεται μεταφορά όλων των στοιχείων


amortized stacks: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/using2stacks(amortized).png

amortized time with 2 stacks -> O(1) αυτό είναι και το πλεονέκτημα αυτής της υλοποίησης


real stacks: https://github.com/chatziko-k08/2022-project-2-vaggeliskg/blob/master/programs/graphs/using2stacks(real).png

στο real time όμως παρατηρούνται τα κατακόρυφα τμήματα που οφείλονται στις περιπτώσεις όπου η 2η στοίβα αδείαζει και χρείαζεται μεταφορά όλων των στοχείων από την 1η σε αυτή που όπως φαίνεται συμβαίνει σπάνια. 

-ΑΣΚΗΣΗ 4: περιέχει 2 συναρτησεις που κατασκευάζουν ένα ισοζυγισμένο δέντρο



### Documentation

Συμπληρώστε εδώ __όσο documentation χρειάζεται__ ώστε οι βαθμολογητές να
κατανοήσουν πλήρως τις λύσεις σας και να τις βαθμολογήσουν ανάλογα. Αυτό θα
πρέπει να γίνει ανεξάρτητα με το αν ο κώδικάς σας είναι καλά σχολιασμένος,
πράγμα που συνιστάται.



ΑΣΚΗΣΗ 1: λειτουργεί κανονικά με περιέχοντας τα απαραίτητα ελπίζω σχόλια

ΑΣΚΗΣΗ 2: πιο αποδοτική υλοποίηση με 2 στοίβες και τα απαραίτητα σχόλια

ΑΣΚΗΣΗ 3: περιέχει στο programs/queue_benchmark τον κώδικα που υπολογίζει τα steps
Στην περίπτωση amortized προσθέτει κάθε φορά στην εκάστοτε λειτουργία και το άθροισμα όλων των προηγούμενων διαιρώντας με n(ελπίζω αυτό να ήταν το ζητούμενο) 
Στην περίπτωση real δεν προσθέτει τα προηγούμενα απλά υπολογίζει τα εκάστοτε βήματα για τις κλήσεις τις remove σε κάθε υλοποίηση
πέρα απο τα λινκ των εικόνων που συμπεριλαμβάνονται στο README.md υπάρχει και το programs/graphs που έχει επίσης τις εικόνες των διγραμμάτων!

ΑΣΚΗΣΗ 4: Υπάρχει η υλοποίηση της set_create_from_sorted_values + μιας set_rec που αναδρομικά κατασκευάζει το σετ



