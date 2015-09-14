while [ true ]
do
	#./leptgraypng >x.png ; sudo convert x.png -normalize -contrast -sharpen 5 -resize 320x240 y.jpg
	./leptgraypng >x.png ; sudo convert x.png -normalize -contrast -sharpen 5 y.jpg
	sleep .05
done
