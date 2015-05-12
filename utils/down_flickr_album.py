
import sys
import ConfigParser
from flickrapi import FlickrAPI
from flickrapi.exceptions import FlickrError
from django.template.defaultfilters import slugify
import requests
import os

def get_flickr(config_file):
	config = ConfigParser.ConfigParser()
	config.read(config_file)

	api_key = config.get('Flickr', 'api_key')
	api_secret = config.get('Flickr', 'api_secret')
	api = FlickrAPI(api_key, api_secret, cache=True)
	if not api.token_valid(perms=u'read'):
		api.get_request_token(oauth_callback='oob')
                authorize_url = api.auth_url(perms=u'read')
                verifier = unicode(raw_input('Verifier code for %s:' % authorize_url))
                api.get_access_token(verifier)
	return api


def download_photo(photo, path):
	title = photo.get('title')
	photo_url = u'https://farm%(farm)s.staticflickr.com/%(server)s/%(id)s_%(secret)s_o.jpg' % {'farm': photo.get('farm'),
												   'server': photo.get('server'),
												   'id': photo.get('id'),
                                                                                                   'secret': photo.get('secret')}
	id = photo.get('id')
	filename = os.path.join(path, "%s.jpg" % id)
	with open(filename, 'wb') as handle:
		response = requests.get(photo_url, stream=True)

		if not response.ok:
			# Something went wrong
			pass

		for block in response.iter_content(1024):
        		if not block:
            			break
        		handle.write(block)
	return id, title

def download_album(api, album_id, path):
	print("Download album to %r" % path)
	filename = os.path.join(path, "title.txt")
	with open(filename, 'w') as filetitle:
	        for photo in api.walk_set(album_id):
			id, title = download_photo(photo, path)
			filetitle.write("%s;%s\n" % (id, title))
			print(title)


if __name__ == '__main__':
	if len(sys.argv) != 3:
		sys.exit(u"Usage: down_flickr_album <secret.cfg> <to/path>")
	config_file = sys.argv[1]
	path = sys.argv[2]

	flickr = get_flickr(config_file)
	album_id = u'72157646514824657'
	download_album(flickr, album_id, path)

