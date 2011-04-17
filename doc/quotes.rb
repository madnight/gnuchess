#!/usr/bin/ruby

# quotes.rb
#
#  Copyright (C) 2001-2011 Free Software Foundation, Inc.
#
#  Copying and distribution of this file, with or without modification,
#  are permitted in any medium without royalty provided the copyright
#  notice and this notice are preserved.
#
#  Contact: bug-gnu-chess@gnu.org

# This script replaces inner double quotes in fi by single quotes and writes
# the result on fo. This is necessary because command 'book add' does not 
# accept more than two matching double quotes in PGN files.

# The program expects exactly two arguments in the command-line.
# The first argument is the name of the input file, i.e. the file that is 
# going to be processed.
# The second argument is the name of the output file, i.e. the file that will
# be written as a result of processing the input file.

# Check the number of command-line arguments: if other than two, display the
# usage message and exit.
if ARGV.size != 2 then
  puts "Usage: ./quotes <pgn_file_input> <pgn_file_output>"
  exit
end

# 'fiName' takes the first command-line argument, i.e. the input file name
fiName = ARGV[0]

# 'foName' takes the second command-line argument, i.e. the output file name
foName = ARGV[1]

# If the input does not exists, print an error message and exit.
if not File.exists? fiName then
  puts "File #{fiName} does not exist."
  exit
end

# If the input and output file names are identical, print an error message
# and exit.
if foName == fiName then
  puts "Provide different input and output file names"
  exit
end

# If the output file already exists, ask the user if she or he wants to 
# replace it. Repeat the question in an infinite loop until the user enters
# a valid answer:
# if the user answers 'y' or 'Y', the program will continue;
# if the user answers 'n' or 'N', the program will exit.
if File.exists? foName then
  answer = ""
  while answer != "y" and answer != "n"
    puts "File #{foName} already exists. Replace it (y/n)?"
    answer = STDIN.gets.chop
    answer.downcase!
  end
  if answer == "n" then
    exit
  end
end

# Open the input file in read-only mode
fi = File.new( fiName, 'r' )

# Open the output file in read-write mode
fo = File.new( foName, 'w' )

# Loop over each line of the input file
fi.each {|line|

  nQuotes = line.count( '\"' )  # Get the number of double quotes that are 
                                # present in the line. 
  oLine = ''     # Initialise the output line
  oLine << line  # By default, the output line is equal to the input line

  # If the number of double quotes in the line is greater than 2, the output 
  # line requires special processing.
  if nQuotes > 2 then

    # The line will be scanned character by character...

    nq = 0  # Initialize the count of double quotes in the line.
    oLine = ''  # Overrides previous value of the output line

    # Loop over each character of the line
    line.each_char {|c|
      # If the character is a double quote, do special processing;
      # otherwise, append it to the output line.
      if c == "\"" then
        # Two special cases are considered (enough to cover book_1.01.pgn):
        #   a) 3 double quotes => replace the 2nd double quote by a single quote
        #   b) 4 double quotes => replace the 2nd and 3rd double quotes  by a 
        #      single quote
        # Otherwise, append the character to the output line.
        if ( nQuotes == 3 and nq == 1 ) or ( nQuotes == 4 and ( nq == 1 or nq == 2 ) ) then
          oLine << '\''
        else
          oLine << c
        end
        nq += 1
      else
        oLine << c
      end
    }  # End of looping over the characters of a line

  end

  fo.puts oLine  # Write output line to the output file

}  # End of looping over the lines of the input file

# Close the output file
fo.close

# Close the input file
fi.close
