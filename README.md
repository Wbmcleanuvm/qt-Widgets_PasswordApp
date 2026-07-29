# Qt widgets Password Storage project

### Languages used
* c++
### Safety
>This project features a non-api hard coded implementation of the PBKDF2-SHA256 hashing algorithm which is used along with a salt generated from OpenSSL's random function to safely store the users master password which in turn unlocks the users other inputted passwords. All passwords entered by the user are encrypted and stored in an Sqlite3
database. The encryption and decryption utilizes OpenSSL's AES_256 function along with the users master key as the function key.

### Features
> This lightweight password database is perfect for any desktop, it guarantees safety and that your passwords won't be shared anywhere without your doing so. Along with this the ui is very slick and easy to navigate on the go.
### Installation
> Currently, there is only a Windows installer but since this app is mostly c++ a apple installer could easily be made aswell \n
> Installation link https://drive.google.com/file/d/11mWO3c2IgwnexFm4J_52NRuJj72osDC-/view?usp=sharing

