# this requires sphinx to be installed:
# $ easy_install -U sphinx
make html
git checkout gh-pages
rm -r glps _sources _static
mv docs_build/html/* .
git add .
git commit -am "build $(date)"
git push origin gh-pages
git checkout master
