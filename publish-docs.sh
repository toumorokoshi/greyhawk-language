# this requires sphinx to be installed:
# $ easy_install -U sphinx
make html
git checkout gh-pages
rm -rf _*
mv docs_build/* .
git add .
git commit -am "build $(date)"
git push origin gh-pages
git checkout master
