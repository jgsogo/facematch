
import sys
import os


def build_csv(directory):
              print(u'Building CSV file for directory %r' % directory)
              label = 0
              filename = os.path.basename(directory)
              filename = os.path.join(directory, '../', '%s.csv' % filename)
              f = open(filename, 'w')              
              for root, dirs, files in os.walk(directory):
                            for name in files:
                                          path = os.path.abspath(os.path.join(root, name))
                                          f.write('%s;%d\n' % (path, label))
                            label += 1
              f.close()

              
if __name__ == '__main__':
              if len(sys.argv) != 2:
                            sys.exit(u"Usage 'build_csv <path/to/image/directory>'")
              directory = sys.argv[1]
              build_csv(directory)