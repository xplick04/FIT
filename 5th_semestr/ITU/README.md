# Animal Shelter ITU
## Datum: 8.12.2022
## Autoři projektu
* Jan Zdeněk (xzdene01)
* Maxim Plička (xplixk04)
* Tomáš Vitula (xvitul03)

## Odevzdané soubory
Vzhledem k tomu, že jsme každý vypracovali vlastní frontendovou aplikaci, tak jsou zdrojové kódy ve třech složká pojmenovaných podle loginu.
  
## Popis instalace pro Windows (ve složkách xzdene01 a xplick04)
* `python -m venv env` (vytvoření virtuálního prostředí)
* `.\env\Scripts\activate` (aktivace viruálního prostředí)
* `pip install -r requirements.txt` (stažení všech potřebných modulů pro Django aplikaci)
* `npm install` (stažení všech potřebných modulů pro React aplikaci a sestavení aplikace)
* `copy build/favicon.ico build/static/favicon.ico` (zpřístupní favicon.ico)
* `copy build/manifest.json build/static/manifest.json` (zpřístupní manifest.json)
* `python manage.py migrate` (migrace databáze)
* `python manage.py runserver` (spuštění lokálního serveru)

Aby aplikace správně fungovala, tak je využito backendu, který je nahrán na Heroku i s ukázkovými daty. Na lokálním serveru tudíž jede pouze React aplikace a je možno ignorovat vygenerovanou sqlite databázi.

Pro xvitul03 není žádná instalace, složka obsahuje pouze zdrojové kódy k React aplikaci.