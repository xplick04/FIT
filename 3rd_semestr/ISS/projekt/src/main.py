import matplotlib.pyplot as plt
import numpy as np
import soundfile as sf
import math
from statistics import mean
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk

#------------------------------Ukol 1------------------------------------------------------
signal, fs = sf.read('../audio/xplick04.wav')
t = np.arange(signal.size) / fs

plt.figure(figsize=(6, 3))
plt.plot(t, signal)

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_ylabel('$Amplituda$')
plt.gca().set_title('Vstupní zvukový signál')
plt.tight_layout()
#plt.savefig("../images/1.png", format="png")
plt.close()

print("Max: ", max(signal))
print("Min: ", min(signal))
print("Fs:", fs)
print("N: ", len(signal))
print("Délka[s]: ", len(signal) / fs)

#------------------------------Ukol 2------------------------------------------------------
def ustredneni(signal):
    #výpočet střední hodnoty
    meanValue = mean(signal)
    #ustřednění signálu
    signal -= meanValue
    return signal / max(abs(max(signal)), abs(min(signal)))

signal = ustredneni(signal)

#rozdělení signnálu na překrývající se rámce
def ramec_prevod(signal):
    frames = []
    i = 0
    while i + 1024 <= len(signal):
        frame = signal[i : i+1024]
        frames.append(frame)
        i += 512
    return frames[42]

vybrany_ramec = ramec_prevod(signal)

tFrame = np.arange(1024) / fs
plt.plot(tFrame, vybrany_ramec)
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_ylabel('$Amplituda$')
plt.gca().set_title('Rámec 42')
#plt.savefig("../images/2.png", format="png")
plt.close()

#------------------------------Ukol 3------------------------------------------------------
#DFT
def fft(ramec):
    N = len(ramec)
    dft_array = []
    for k in range(N-1): #od 0 do N-1
        a = 0
        for n in range(N-1): #suma od 0 do N-1
            exp = np.exp(-2j * np.pi * k * n / N)
            a += ramec[n] * exp
        dft_array.append(a)
    return dft_array

freqData = fft(vybrany_ramec)
freqData = freqData[:512] #oriznuti zaporne strany

tFrame = np.arange(0, 8000, 8000/512)
plt.plot(tFrame, np.abs(freqData))
plt.gca().set_xlabel('$f[Hz]$')
plt.gca().set_ylabel('$Koeficient$')
plt.gca().set_title('DFT Rámec 42')
#plt.savefig("../images/3.png", format="png")
plt.close()

#------------------------------Ukol 4------------------------------------------------------
#spektogram
f, t, sgr = spectrogram(signal, fs)
sgr_log = 10 * np.log10(sgr+1e-20)

plt.figure(figsize=(9, 3))
plt.pcolormesh(t, f, sgr_log)
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
plt.gca().set_title('logaritmický výkonový spektrogram')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
plt.tight_layout()
#plt.savefig("../images/4.png", format="png")
plt.close()

#------------------------------Ukol 5------------------------------------------------------
#určení rušivých frekvencí z DFT
f1 = 675
f2 = 1350
f3 = 2025
f4 = 2700

print("Frekvence rušivého signálu: f1: ", f1, "Hz, f2: ", f2, "Hz, f3: ", f3, "Hz, f4: ", f4, "Hz")

#------------------------------Ukol 6------------------------------------------------------
#generovani rušivého signálu
def cos_generate():
    arg1, arg2, arg3, arg4, cos1, cos2, cos3, cos4, cos_final = [], [], [], [], [], [], [], [], []
    arg1 = np.linspace(0, 2 * np.pi * f1 * (len(signal)/fs), len(signal))
    arg2 = np.linspace(0, 2 * np.pi * f2 * (len(signal)/fs), len(signal))
    arg3 = np.linspace(0, 2 * np.pi * f3 * (len(signal)/fs), len(signal))
    arg4 = np.linspace(0, 2 * np.pi * f4 * (len(signal)/fs), len(signal))

    for i in range(len(signal)):
        cos1.append(math.cos(arg1[i]))
        cos2.append(math.cos(arg2[i]))
        cos3.append(math.cos(arg3[i]))
        cos4.append(math.cos(arg4[i]))
        cos_final.append((cos1[i] + cos2[i] + cos3[i] + cos4[i]) * 0.25)

    return cos_final

cos_final = cos_generate()

sf.write('../audio/4cos.wav', cos_final, fs)

cos_data, cos_fs = sf.read('../audio/4cos.wav')

f, t, sgr = spectrogram(cos_data, cos_fs)
sgr_log = 10 * np.log10(sgr+1e-20)

plt.figure(figsize=(9, 3))
plt.pcolormesh(t, f, sgr_log)
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
plt.gca().set_title('Spektrogram signálu ze 4 cosinusovek')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
plt.tight_layout()
#plt.savefig("../images/5.png", format="png")
plt.figure(figsize=(4,3.5))
plt.close()

#------------------------------Ukol 7------------------------------------------------------
#kruhova frekvence
w1 = 2 * np.pi * (f1 / fs)
w2 = 2 * np.pi * (f2 / fs)
w3 = 2 * np.pi * (f3 / fs)
w4 = 2 * np.pi * (f4 / fs)

#nulove body
n1 = np.exp(1j * w1 * 1)
n2 = np.exp(1j * w2 * 2)
n3 = np.exp(1j * w3 * 3)
n4 = np.exp(1j * w4 * 4)

# komplexne sdruzene body
n5 = np.conj(n1)
n6 = np.conj(n2)
n7 = np.conj(n3)
n8 = np.conj(n4)

#koeficienty filtru
filter_array = (n1,n2,n3,n4,n5,n6,n7,n8)

#impulsni odezva filtru
a = [1, 0, 0, 0, 0, 0, 0, 0, 0]
b = np.poly(filter_array)

#impulsni odezva
N_imp = 32
imp = [1, *np.zeros(N_imp-1)]
h = lfilter(b, a, imp)

#frekvencni charakteristika
w, H = freqz(b, a)

#nuly, poly
z, p, k = tf2zpk(b, a)

#stabilita
is_stable = (p.size == 0) or np.all(np.abs(p) < 1)
print('Filtr {} stabilní.'.format('je' if is_stable else 'není'))

#filtrace
sf = lfilter(b, a, signal)
f, t, sfgr = spectrogram(sf, fs)
sfgr_log = 10 * np.log10(sfgr+1e-20)
plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('Impulsní odezva $h[n]$')
plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
#plt.savefig("../images/6.png", format="png")
#plt.show()
plt.close()

#------------------------------Ukol 8------------------------------------------------------

#nuly a poly
p = np.roots(a)
z = np.roots(b)

plt.figure(figsize=(4,3.5))
#jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

#nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_xlabel('Realná složka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarní složka $\mathbb{I}\{$z$\}$')
plt.gca().set_title('nulové body a póly navrženého  filtru')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper right')

plt.tight_layout()
#plt.savefig("../images/7.png", format="png")
#plt.show()
plt.close()

#------------------------------Ukol 9------------------------------------------------------

_, ax = plt.subplots(1, 2, figsize=(8,3))

ax[0].plot(w / 2 / np.pi * fs, np.abs(H))
ax[0].set_xlabel('Frekvence [Hz]')
ax[0].set_title('Modul frekvenční charakteristiky $|H(e^{j\omega})|$')

ax[1].plot(w / 2 / np.pi * fs, np.angle(H))
ax[1].set_xlabel('Frekvence [Hz]')
ax[1].set_title('Argument frekvenční charakteristiky $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
#plt.savefig("../images/8.png", format="png")
#plt.show()
plt.close()