
import sys
import os


def build_csv(directory):
              print(u'Building CSV file for directory %r' % directory)
              label = 0
              filename = os.path.basename(directory)
              filename = os.path.join(directory, '../', '%s.csv' % filename)
              f = open(filename, 'w')
              i = 0
              for root, dirs, files in os.walk(directory):
                            for name in files:
					  if name.lower().endswith(('png', 'jpg', 'jpeg')):
	                                          path = os.path.abspath(os.path.join(root, name))
        	                                  f.write('%d;%s;%d\n' % (i, path, label))
                	                          i += 1
                            label += 1
              f.close()

              
if __name__ == '__main__':
              if len(sys.argv) != 2:
                            sys.exit(u"Usage 'build_csv <path/to/image/directory>'")
              directory = sys.argv[1]
              build_csv(directory)
