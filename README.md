# "Quebrador" de MD5
Trabalho da disciplina de Arquiteturas Paralelas e Distribuídas (APD) do curso de Tecnologia em Sistemas para Internet.
# Utilização
**Compilar:** gcc quebra_md5.c -o quebra_md5 -lcrypto -lssl -fopenmp<br/>
**Executar:** ./quebra_md5 -hash -n_letras _[-nthreads]_
## **Exemplos:**
### Sequencial:
**Entrada:** ./quebra_md5 24b91372fa71abbdf7f69b88834cfaa7 4 0<br/>
**Saída:** No console:
```
Método utilizado: sequencial
String: @T$1
Tempo: ...
```
### Paralelo:
**Entrada:** ./quebra_md5 d63f657df132357091c5b51031bba8bf 5 8<br/>
**Saída:** No console:

```
Método utilizado: paralelismo com 8 threads
String: =@pD?
Tempo: ...
```

### Relatório:
**Entrada:** ./quebra_md5 2ee17599597d02dbf88b829798db0518 3<br/>
**Saída:** No console é impresso a velocidade individual de cada execução, e na pasta onde é executado é salvado um arquivo de formato CSV.
