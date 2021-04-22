import plotly
import sys
from plotly.offline import plot
from sklearn.metrics import mean_squared_error
import pandas as pd
import plotly.graph_objects as go
import numpy as np
from scipy.fftpack import ifft, idct


if __name__ == "__main__":
    df = pd.read_csv("assignment3.csv").set_index('channel\sample').T

    print("afwe")
    data = []
    data.append(
        go.Bar(
            x=df.columns.values,
            y=df.mean().values*(-1),
        )
    )
    fig = go.Figure(
        data=data
    )
    fig.update_layout(title='Measured signal on different 802.15.4 channels',
                      xaxis_title='Channel',
                      yaxis_title='Measured RSSI value')
    plot(fig)