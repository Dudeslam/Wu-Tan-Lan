import plotly
from plotly.offline import plot
from sklearn.metrics import mean_squared_error
import pandas as pd
import plotly.graph_objects as go
import numpy as np
from scipy.fftpack import ifft, idct

def read_file(filename: str):
    f = open(filename, "r")
    raw_file = f.read()
    clean_file = pd.to_numeric(pd.Series(raw_file.split(".\n")).str.rsplit("] = ").str[-1]).dropna()
    return clean_file

def read_original_file(filename: str):
    f = open(filename, "r")
    raw_file = f.read()
    return pd.Series(raw_file.split(","))

if __name__ == "__main__":
    N = 512
    M = 120
    f = read_file("dct512.txt")
    original = pd.to_numeric(read_original_file("Originalmessage.txt"))
    test = pd.Series(idct(f.values[0:M], n=N))/N
    data = []
    data.append(
        go.Scatter(
            x=test.index,
            y=test.values,
            mode='markers',
            name="Reconstructed signal"
        )
    )
    data.append(
        go.Scatter(
            x=original.index,
            y=original.values,
            mode='lines',
            name="Original signal"
        )
    )
    mse = mean_squared_error(original.values, test.values)
    fig = go.Figure(
        data=data
    )
    fig.update_layout(title='ECG signal reconstructed(M={}) and original signal(N={}) with MSE on: {}'.format(M,N,mse),
                      xaxis_title='Sample NO',
                      yaxis_title='Voltage[V]')
    plot(fig)



