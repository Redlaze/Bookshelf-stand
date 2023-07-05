import streamlit as st
import socket
import clickhouse_connect
import pandas as pd
import time
import matplotlib.pyplot as plt
import numpy as np
def main():
    # if 'prev_slider_value' not in st.session_state:
    #     st.session_state.prev_slider_value = None
    # clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # clientsocket.connect(('192.168.31.142', 3335))
    client = clickhouse_connect.get_client(host='89.108.99.89', username='default', port='8123', password='qwerty')
    m = int(client.command('SELECT * FROM first_start WHERE Id==(SELECT max(Id) FROM first_start)')[0])
    # def program1():
    #     while True:
    #         data = client.command('SELECT * FROM mot_table WHERE Id==(SELECT max(Id) FROM mot_table)')
    #         temp_1 = data[0]
    #         temp_2 = data[1]
    #         temp_3 = data[2]
    #         amperage = data[3]
    #         frequency = data[4]
    # def program2():
    #     temp_1 = 0
    #     temp_2 = 0
    #     temp_3 = 0
    #     amperage = 0
    #     frequency = 0
    #     '''
    #     import clickhouse_connect
    #
    #     client = clickhouse_connect.get_client(host='89.108.99.89', username='default',
    #                                            port='8123', password='qwerty')
    #     client.command('CREATE TABLE table (Timestamp DateTime64, Direction String, Speed UInt32, Frequency UInt32) '
    #                    'ENGINE MergeTree ORDER BY Timestamp')
    #     '''
    data = client.command('SELECT * FROM mot_table WHERE Id==(SELECT max(Id) FROM mot_table)')
    all_data = client.command('SELECT * FROM mot_table')
    temp_1 = data[1]
    temp_2 = data[2]
    temp_3 = data[3]
    amperage = data[4]
    freq = data[5]
    vol = data[6]
    # st.write('''# Направление движения''')
    # direction = st.selectbox('', ['Вправа', 'Влево'])
    st.write('''# Частота вращения''')
    frequency = st.slider('', 0, 526)
    fr = frequency
    # if frequency != st.session_state.prev_slider_value:
        # Обновление предыдущего значения ползунка
        # st.session_state.prev_slider_value = frequency
    print("Отправляю данные")
    m += 1
    # clientsocket.send(str(frequency).encode('utf-8'))
    row = [[m, int(frequency)]]
    try:
        client.insert('first_start', row, column_names=['Id', 'frequency'])
    except (clickhouse_connect.driver.exceptions.Error, clickhouse_connect.driver.exceptions.OperationalError):
        print("DatabaseError")
    else:
        df = pd.DataFrame({'Температура катушки 1': [temp_1], 'Температура катушки 2': [temp_2], 'Температура статора': [temp_3],
                                    'Ток (мА)': [amperage], 'Частота (Гц)': [freq], 'Напряжение (В)':[vol]})
        temp_1_data = np.array(all_data[1::6]).astype(float)
        temp_2_data = np.array(all_data[2::6]).astype(float)
        temp_3_data = np.array(all_data[3::6]).astype(float)
        amperage_data = np.array(all_data[4::6]).astype(float)
        fig, ax = plt.subplots(figsize=(20, 15))
        ax.plot(temp_1_data, label='Температура катушки 1')
        ax.plot(temp_2_data, label='Температура катушки 2')
        ax.plot(temp_3_data, label='Температура статора')
        ax2 = ax.twinx()
        ax2.plot(amperage_data, label='Ток', color='r', linewidth=2.5)
        ax.legend(fontsize=30)
        ax2.legend(fontsize=30)
        step = 0.1
        ax.set_yticks(np.arange(min(temp_1_data), max(temp_1_data)+step, step))
        ax.set_xlabel('время')
        ax.set_ylabel('Температура')
        df_html = df.style.set_properties(**{'font-size': '20px'}).render()
        # st.dataframe(df, 1000, 1000)
        st.markdown(
                """
                <style>
                table.dataframe td, table.dataframe th {
                    font-size: 1000px;
                }
                </style>
                """,
                unsafe_allow_html=True
            )
        st.write(df_html, unsafe_allow_html=True)
        st.pyplot(fig)
        st.write('<style>div[data-baseweb="table"] table { font-size: 100px; }</style>', unsafe_allow_html=True)
     #   st.experimental_rerun()
if __name__ == "__main__":
    main()
#     thread1 = threading.Thread(target=program1)
#     # thread2 = threading.Thread(target=program2)
#
#     thread1.start()
#     # thread2.start()
#
#     #date_time = pd.to_datetime(client.command("SELECT now('Europe/Moscow')"))
