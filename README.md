# LSB-Steganography
The soul purpose of this project is to hide the data(secret text) in a image(.bmp file) and send it to the authorised person so that the authorosed person can only access the file with password.
Achieved both encoding the secret text into an image fileand decoding the secret text 
Steganography is a technique of concealing a message within another message or a physical object.
I have used LSB staganography method. In this type of steganography, the information hider embeds the secret information in the least significant bits of a media file.altering the least bits will node modify the image to visible extent.   
In this project .bmp file was used to hide the secret text because bmp is a uncomressed file which can be accessed easily. other image files such as .jpg or .png are compressed files.
Magic string has been used to identify thesecretly embedded files among other files. when the authorised person vviews the files, he/she should recognise the magic string which acts as a password and then he can decode the text from the image
Encoded the secret file extension so that we know how much data has to be decoded.
