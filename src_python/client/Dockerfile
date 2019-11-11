FROM python:3.6.8


COPY requirements.txt ./
RUN pip install --upgrade pip
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD PYTHONPATH="$PYTHONPATH:$PWD/generated" python ./client.py $ID $SERVER_KEY $POSY $POSX $MAP