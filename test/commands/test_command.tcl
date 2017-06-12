#!/usr/bin/tclsh

package require Expect

variable gnuchessplayer
variable n_pass
variable n_fail
variable failed_test_cases

proc pass { } {
    set ::n_pass [expr $::n_pass + 1]
    puts " ----> PASS"
}

proc fail {test_case_title} {
    set ::n_fail [expr $::n_fail + 1]
    lappend ::failed_test_cases $test_case_title
    puts " ----> FAIL"
}

proc test_setup { } {
}

proc test_cleanup { } {
    #send -i $::gnuchessplayer "quit"
    #close $::gnuchessplayer
}

proc test_case {title steps} {
    puts "---------------------------------------------------------------------"
    #test_setup
    if {[lindex $steps 0] == "xboard"} {
        spawn ../../src/gnuchess -x; set ::gnuchessplayer $spawn_id
        set steps [lreplace $steps 0 0]
    } else {
        spawn ../../src/gnuchess; set ::gnuchessplayer $spawn_id
    }
    set test_pass 1
    puts -nonewline $title
    foreach step $steps {
        if {[string first "I " $step] == 0} {
            send -i $::gnuchessplayer "[string range $step 2 end]\n"
        } elseif {[string first "O " $step] == 0} {
            expect {
                -i $::gnuchessplayer [string range $step 2 end] {}
                timeout {set test_pass 0}
            }
        } else {
            set test_pass 0
        }
    }
    if {$test_pass == 1} {
        pass
    } else {
        fail $title
    }
    #test_cleanup
    close
    #after 2000
}

# Test suite
proc run { } {

    ############################################################################
    # Test cases for individual commands
    ############################################################################

    test_case "Name and version" [list "O GNU Chess 6.2"]
    test_case "Invalid move" [list "I " "O Invalid move:" "O White"  "I e5" "O Invalid move:" "O White"]
    test_case "Command: ?" [list "I ?" "O White (1)"]
    test_case "Command: accepted" [list "I accepted" "O White (1)"]
    test_case "Command: activate" [list "I activate" "O currently not supported" "O White (1)"]
    test_case "Command: analyze" [list "I analyze" "O White (1)" "O 1 +" "O 2 +"]
    test_case "Command: bk" [list "I bk" "O %)" "O White (1)"]
    test_case "Command: bk" [list "I bk" "O %)" "O White (1)"]
    test_case "Command: black" [list "I black" "O currently not supported" "O White (1)"]
    test_case "Command: white" [list "I white" "O currently not supported" "O White (1)"]
    test_case "Command: book" [list "I book" "O Incorrect book option" "O White (1)"]
    # TODO book commands
    test_case "Command: depth 1" [list "I depth 1" "O Search to a depth of 1" "O White (1)"]
    test_case "Command: depth 5" [list "I depth 5" "O Search to a depth of 5" "O White (1)"]
    test_case "Command: easy" [list "I easy" "O White (1)"]
    test_case "Command: hard" [list "I hard" "O White (1)"]
    # TODO Add more complex cases for hard and post
    test_case "Command: edit" [list "xboard" "I edit" "O not implemented"]
    test_case "Command: edit" [list "I xboard" "I edit" "O not implemented"]
    test_case "Command: epdload" [list "I epdload" "O Error reading file ''" "O White (1)"]
    test_case "Command: epdload" [list "I epdload ./Polgar-Karpov.epd" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (1)"]
    test_case "Command: load" [list "I load ./Polgar-Karpov.epd" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (1)"]
    test_case "Command: epdsave" [list "I epdsave" "O Invalid filename" "O White (1)"]
    file delete -force ./tmp.epd
    test_case "Command: epdsave" [list "I epdload ./Polgar-Karpov.epd" "O . . . . R . K ." "I depth 1" "I Kh1" "O . . . . R . . K" "O White (2)" "I epdsave ./tmp.epd" "O White (2)" "I epdload ./tmp.epd" "O . . . . R . . K"]
    file delete -force ./tmp.epd
    test_case "Command: save" [list "I load ./Polgar-Karpov.epd" "O . . . . R . K ." "I depth 1" "I Kh1" "O . . . . R . . K" "O White (2)" "I save ./tmp.epd" "O White (2)" "I epdload ./tmp.epd" "O . . . . R . . K"]
    file delete -force ./tmp.epd
    test_case "Command: exit" [list "I exit" "O "]
    test_case "Command: force" [list "I force" "O White (1)" "I e4" "O . . . . P . . ." "O Black (1)"]
    test_case "Command: manual" [list "I manual" "O White (1)" "I e4" "O . . . . P . . ." "O Black (1)"]
    test_case "Command: go" [list "I go" "O Thinking..." "O Black (1)" "I go" "O Thinking..." "O White (2)"]
    # TODO Add more complex cases for go
    test_case "Command: graphic" [list "I graphic" "O Graphic mode is enabled." "O White (1)"]
    test_case "Command: nographic" [list "I nographic" "O Graphic mode is disabled." "O White (1)"]
    test_case "Command: hash" [list "I hash" "O Hashing is on." "O White (1)" "I hash off" "O Hashing is off." "O White (1)" "I hash" "O Hashing is off." "O White (1)" "I hash on" "O Hashing is on." "O White (1)"]
    test_case "Command: help" [list "I help" "O List of commands: (help COMMAND to get more help)" "O White (1)" "I help help" "O help" "O Produces a help blurb corresponding to this list of commands." "O White (1)"]
    # TODO FIX test_case "Command: hint" [list "I hint" "O Hint:" "O White (1)"]
    test_case "Command: ics" [list "I ics" "O " "O White (1)"]
    test_case "Command: pgnload" [list "I pgnload ./Polgar-Karpov.pgn" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (1)"]
    file delete -force ./tmp.pgn
    # TODO FIX test_case "Command: pgnsave" [list "I pgnload ./Polgar-Karpov.pgn" "O . . . . R . K ." "I depth 1" "I Kh1" "O . . . . R . . K" "O White (35)" "I pgnsave ./tmp.pgn" "O White (35)" "I pgnload ./tmp.pgn" "O . . . . R . . K"]
    file delete -force ./tmp.pgn
    test_case "Command: pgnreplay" [list "I pgnreplay ./Polgar-Karpov.pgn" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O white  KQkq" \
                               "O r n b q k b n r" \
                               "O p p p p p p p p" \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O P P P P P P P P" \
                               "O R N B Q K B N R" \
                               "O White (1)" \
                           "I first" \
                               "O white  KQkq" \
                               "O r n b q k b n r" \
                               "O p p p p p p p p" \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O P P P P P P P P" \
                               "O R N B Q K B N R" \
                               "O White (1)" \
                           "I next" \
                               "O 1. e4" \
                               "O black  KQkq  e3" \
                               "O r n b q k b n r" \
                               "O p p p p p p p p" \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . P . . ." \
                               "O . . . . . . . ." \
                               "O P P P P . P P P" \
                               "O R N B Q K B N R" \
                               "O Black (1)" \
                           "I n" \
                               "O 1. c6" \
                               "O white  KQkq" \
                               "O r n b q k b n r" \
                               "O p p . p p p p p" \
                               "O . . p . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . P . . ." \
                               "O . . . . . . . ." \
                               "O P P P P . P P P" \
                               "O R N B Q K B N R" \
                               "O White (2)" \
                           "I last" \
                               "O 33. Qe7" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (34)" \
                           "I previous" \
                               "O 33. hxg5" \
                               "O black" \
                               "O . . r . . n k ." \
                               "O . . q . . p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O Black (33)" \
                           "I p" \
                               "O 32. hxg5" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . q . . p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . p ." \
                               "O P p N P . . . P" \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (33)"]
    test_case "Command: level" [list "I level 40 1 2" \
                               "O Time Control: 40 moves in 60.00 secs" \
                               "O Fischer increment of 2 seconds" \
                               "O White (1)"]
    test_case "Command: list & name & result" [list "I list ?" \
                               "O name    - list known players alphabetically" \
                               "O score   - list by GNU best result first" \
                               "O reverse - list by GNU worst result first" \
                               "O White (1)" \
                               "I name Stuart" "I result" "I name Simon" "I result" \
                               "I list name" "O Simon" "O Stuart" "O White (1)"]
    file delete -force ./log.000
    file delete -force ./log.001
    file delete -force ./game.000
    file delete -force ./game.001
    file delete -force ./players.dat
    test_case "Command: memory" [list "I memory" "O Current HashSize is 256 MB" "O White (1)" \
                                      "I memory 128" "O Current HashSize is 128 MB"]
                                      # TODO FIX "I memory 128" "O Current HashSize is 128 MB" "O White (1)"]
    test_case "Command: new" [list "I new" "O White (1)" "I depth 1" "I e4" "O White (2)" "I new" "O White (1)"]
    # TODO FIX test_case "Command: post & nopost" [list "I post" "O 1 +" "O White (1)" "I nopost" "O White (1)"]
    test_case "Command: post & nopost" [list "I depth 1" "I post" "O White (1)" "I e4" "O White (2)" "I nopost" "O White (2)"]
    test_case "Command: null" [list "I null" "O Null-move heuristic is on." "O White (1)" "I null off" "O Null-move heuristic is off." "O White (1)" "I null" "O Null-move heuristic is off." "O White (1)" "I null on" "O Null-move heuristic is on." "O White (1)"]
    test_case "Command: otim" [list "I otim 1" "O White (1)"]
    test_case "Command: ping" [list "I ping" "O pong" "O White (1)"]
    # TODO FIX test_case "Command: protover" [list "xboard" "I protover" "O feature"]
    test_case "Command: quit" [list "I quit" "O "]
    test_case "Command: random" [list "I random" "O currently not supported" "O White (1)"]
    test_case "Command: rating" [list "I rating" "O my rating = 0, opponent rating = 0" "O White (1)"]
    test_case "Command: rejected" [list "I rejected" "O "]
    # FIX test_case "Command: remove" [list "I e4" \
                               #"O . . . . P . . ." \
                               #"O . . . . . . . ." \
                               #"O P P P P . P P P" \
                               #"O R N B Q K B N R" \
                               #"O White (2)" \
                           #"I remove" \
                               #"O . . . . . . . ." \
                               #"O . . . . . . . ." \
                               #"O P P P P . P P P" \
                               #"O R N B Q K B N R" \
                               #"O White (1)"]
    test_case "Command: setboard" [list "I setboard 2r2nk1/4qpp1/4p3/p2nN1P1/PpNP4/1PrR1Q2/5P2/4R1K1 w - - bm 1; id 1;" "O White (1)" "I show board" \
                               "O white" \
                               "O . . r . . n k ." \
                               "O . . . . q p p ." \
                               "O . . . . p . . ." \
                               "O p . . n N . P ." \
                               "O P p N P . . . ." \
                               "O . P r R . Q . ." \
                               "O . . . . . P . ." \
                               "O . . . . R . K ." \
                               "O White (1)"]
    test_case "Command: show" [list \
                           "I show board" \
                               "O white  KQkq" \
                               "O r n b q k b n r" \
                               "O p p p p p p p p" \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O P P P P P P P P" \
                               "O R N B Q K B N R" \
                               "O White (1)" \
                           "I show rating" \
                               "O My rating =" \
                               "O Opponent rating =" \
                               "O White (1)" \
                           "I show time" \
                               "O White (1)" \
                           "I show moves" \
                               "O Na3" "O No. of moves generated =" \
                           "I show escape" \
                               "O No. of moves generated =" \
                           "I show noncapture" \
                               "O Na3" "O No. of moves generated =" \
                           "I show capture" \
                               "O No. of moves generated =" \
                           "I show eval" \
                               "O currently not supported" \
                           "I show score" \
                               "O currently not supported" \
                           "I e4" "I show game" \
                               "O White   Black" "O 1.  e4" "O White (2)" \
                           "I show pin" \
                               "O currently not supported"]
    test_case "Command: solve" [list "I depth 1" "I solve ./Polgar-Karpov.epd" "O Best move =" "O My move is :" "O Incorrect" "O Correct=" "O White (1)"]
    test_case "Command: solveepd" [list "I depth 1" "I solveepd ./Polgar-Karpov.epd" "O Best move =" "O My move is :" "O Incorrect" "O Total=" "O White (1)"]
    test_case "Command: st" [list "I st 1" "O White (1)"]
    test_case "Command: switch" [list "I switch" "O currently not supported" "O White (1)"]
    test_case "Command: test" [list "I test" "O currently not supported" "O White (1)"]
    test_case "Command: time" [list "I time 1" "O White (1)"]
    test_case "Command: undo" [list "I depth 1" "I e4" \
                               "O . . . . P . . ." \
                               "O . . . . . . . ." \
                               "O P P P P . P P P" \
                               "O R N B Q K B N R" \
                               "O White (2)" \
                           "I undo" \
                               "O . . . . P . . ." \
                               "O . . . . . . . ." \
                               "O P P P P . P P P" \
                               "O R N B Q K B N R" \
                               "O Black (1)" \
                           "I undo" \
                               "O . . . . . . . ." \
                               "O . . . . . . . ." \
                               "O P P P P P P P P" \
                               "O R N B Q K B N R" \
                               "O White (1)"]
    test_case "Command: usage" [list "I usage" "O Usage:" "O Options:" "O bug-gnu-chess@gnu.org" "O White (1)"]
    test_case "Command: usermove" [list "I usermove e4" "O White (2)"]
    test_case "Command: variant" [list "I variant" "O White (1)"]
    test_case "Command: version" [list "I version" "O GNU Chess 6" "O White (1)"]
    test_case "Command: xboard" [list "I depth 1" "I e4" "O move" "I new" "I xboard off" "I e4" "O My move is :" "O White (2)" "I new" "O White (1)" "I xboard on" "I e4" "O move"]

    # Print results
    puts "====================================================================="
    puts "\nTests results:\n\tPASS: $::n_pass\n\tFAIL: $::n_fail"
    if {$::n_fail == 0} {
        puts "  OK\n"
    } else {
        puts "  FAILURE\n"
        foreach title $::failed_test_cases {
            puts "\t$title"
        }
    }
}

set n_pass 0
set n_fail 0
set failed_test_cases [list]
set timeout 2
run
