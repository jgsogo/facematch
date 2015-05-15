
import os
import sys
from google_download.gistfile1 import go


if __name__=='__main__':
	if len(sys.argv) != 2:
		print(u'usage: %s <path>' % sys.argv[0])
		exit(1)

	path = os.path.abspath(sys.argv[1])
	print(u'Images will be downloaded to %r' % path)
	queries = ['rajoy', 'albert rivera', 'pablo iglesias', 'pedro sanchez', 'rosa diez', 'alberto garzon',]
	for q in queries:
		try:
			print(u'Downloading images from query %r' % q)
			go(q, path)
			print(u'\n')
		except Exception as e:
			print(u'\t - error: %s' % str(e))
