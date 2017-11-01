# TUCIL-1 IF3130 ***CIDR Calculator***
Oleh : Azis Adi Kuncoro / 13515120

## PETUNJUK PENGGUNAAN PROGRAM
Gunakan command dibawah untuk compile program

```
make
```

Gunakan command ini untuk menjalankan program

```
make run
```

## PENJELASAN SINGKAT MENGENAI PROSES PENGERJAAN

### Phase-1

* Pada phase-1, diminta memberikan **subnet valid** untuk input host yang diberikan.
* Idenya pada program yang saya buat ialah membuat subnet ``` XXX.XXX.XXX.0/24 ```
dengan ```XXX.XXX.XXX``` merupakan 24 bit pertama dari host.

### Phase-2

* Pada phase-2, diminta memberikan **jumlah host valid** untuk sebuah subnet yang disediakan.
* Idenya adalah dengan memparse string setelah karakter ``` / ``` dan ditampung ke suatu variabel (misal N).
* Kemudian konversi ke integer, dan outputkan hasil berupa **2<sup>32-N</sup>**. 

### Phase-3

* Pada phase-3, diminta memberikan predikat **T** untuk host yang berada disuatu subenet, **F** sebaliknya.
* Idenya adalah dengan melakukan masking untuk subnet dan host. apabila hasil operasi XOR dari hasil masking tadi.
* ``` return 1 ``` jika hasil XOR bernilai 0, dan ``` return 0 ``` jika tidak.
