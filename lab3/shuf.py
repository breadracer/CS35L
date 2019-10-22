#!/usr/bin/python
import random, sys
import argparse

class shuf_io:
    def __init__(self, lines):
        self.lines = lines
        
    def shuffle(self, n):
        if n == -1:
            n = len(self.lines)
        outputs = random.sample(self.lines, n)
        for output in outputs:
            sys.stdout.write(str(output))
            
    def shuffle_repeat(self, n):
        while n != 0:
            sys.stdout.write(str(random.choice(self.lines)))
            n -= 1

            
class shuf_range:
    def __init__(self, start, end):
        self.start = start
        self.end = end

    def shuffle(self, n):
        if n == -1:
            n = self.end - self.start + 1
        outputs = random.sample(range(self.start, self.end + 1), n)
        for output in outputs:
            sys.stdout.write(str(output) + '\n')

    def shuffle_repeat(self, n):
        while n != 0:
            sys.stdout.write(
                str(random.choice
                    (range(self.start, self.end + 1))) + '\n')
            n -= 1

            
def main():
    description_msg1 = "Write a random permutation of the input "
    description_msg2 = """lines to standard output.

With no FILE, or when FILE is -, read standard input."""
    description_msg = description_msg1 + description_msg2
    parser = argparse.ArgumentParser(description=description_msg)
    parser.add_argument("-n", "--head-count", metavar="count")
    parser.add_argument("-i", "--input-range", metavar="lo-hi")
    parser.add_argument("-r", "--repeat", action="store_true")
    parser.add_argument("inputfile", type=argparse.FileType("r"),
                        nargs="?", default=sys.stdin)
    args = parser.parse_args(sys.argv[1:])
    count = args.head_count
    if count == None:
        count = -1
    else:
        try:
            count = int(count)
        except:
            parser.error("invalid NUMLINES: {0}".format(count))
    if args.input_range:
        if args.inputfile != sys.stdin:
            parser.error("extra operand: '{0}'"
                         .format(args.inputfile.name))
        arr = args.input_range.split('-', 1)
        try:
            start = int(arr[0])
        except:
            parser.error("invalid input range: '{0}'".format(arr[0]))
        try:
            end = int(arr[1])
        except:
            parser.error("invalid input range: '{0}'".format(arr[1]))
        if count > end - start + 1:
            parser.error("head counter out of range")    
        try:
            generator_range = shuf_range(start, end)
            if args.repeat:
                generator_range.shuffle_repeat(count)
            else:
                generator_range.shuffle(count)
        except IOError as err:
            parser.error("I/O error({0}): {1}".format(err[0], err[1]))
    else:
        try:
            stream = []
            if args.inputfile == sys.stdin:
                stream = args.inputfile.readlines()
            else:
                f = open(args.inputfile.name)
                stream = f.readlines()
                f.close()
                generator_io = shuf_io(stream)
                if args.repeat:
                    generator_io.shuffle_repeat(count)
                else:
                    generator_io.shuffle(count)
        except IOError as err:
            parser.error("I/O error({0}): {1}"
                         .format(err.errno, err.strerror))

if __name__ == "__main__":
    main()
