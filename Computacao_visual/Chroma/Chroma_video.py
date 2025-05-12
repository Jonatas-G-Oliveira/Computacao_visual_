import cv2
import numpy as np

# -- CHROMA REFS
#Foruns:
# https://stackoverflow.com/questions/10948589/choosing-the-correct-upper-and-lower-hsv-boundaries-for-color-detection-withcv
#
#Documentação:
# https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html
# https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html
# https://docs.opencv.org/4.x/d0/d86/tutorial_py_image_arithmetics.html

#Youtube:
# https://www.youtube.com/watch?v=ozyRoPFneck&ab_channel=CarlosMelo
# https://www.youtube.com/watch?v=l-llIQh0DsI&ab_channel=w3runs
# https://www.youtube.com/watch?v=6PMHKXXhRXc&t=566s&ab_channel=Programa%C3%A7%C3%A3oDin%C3%A2mica

def removerFundoVerde(foreground, background):
    #Criando Janelas
    JANELA_MASK_FOREGROUND = "MASCARA FOREGROUND"
    JANELA_MASK_BACKGROUND = "MASCARA BACKGROUND"
    cv2.namedWindow(JANELA_MASK_FOREGROUND, cv2.WINDOW_GUI_NORMAL)
    cv2.namedWindow(JANELA_MASK_BACKGROUND, cv2.WINDOW_GUI_NORMAL)
    cv2.resizeWindow(JANELA_MASK_FOREGROUND, 400, 300)
    cv2.resizeWindow(JANELA_MASK_BACKGROUND, 400, 300)

    #Convertendo para o sistema de cor HSV
    foreground = cv2.cvtColor(foreground, cv2.COLOR_BGR2HSV)

    # Esticando Background se necessário
    linhas, colunas = foreground.shape[:2]
    bk_linhas, bk_colunas = background.shape[:2]
    if linhas > bk_linhas or colunas > bk_colunas:
        background = cv2.resize(background, (colunas, linhas), interpolation=cv2.INTER_LINEAR)

    #Definindo limites de cor 40 <= hue <= 80
    lower_green = np.array([40, 30, 70], dtype=np.uint8)
    upper_green = np.array([80, 255, 220], dtype=np.uint8)

    #Se o pixel estiver dentro do intervalo ele marca com 255 senao 0
    mascara = cv2.inRange(foreground, lower_green, upper_green)
    cv2.imshow(JANELA_MASK_FOREGROUND,mascara)

    # Cortando o background
    background_cortado = cv2.bitwise_and(background,background, mask=mascara)

    #Invertendo a mascara para mostrar tudo que nao é verde
    mascara_inv = np.invert(mascara)
    cv2.imshow(JANELA_MASK_BACKGROUND, mascara_inv)

    #Cortando o foreground
    foreground_cortado = cv2.bitwise_and(foreground, foreground, mask=mascara_inv)

    #Retornando para bgr e concatenando o background e foreground
    foreground_bgr = cv2.cvtColor(foreground_cortado, cv2.COLOR_HSV2BGR)
    img_final = cv2.addWeighted(background_cortado,1, foreground_bgr, 1, 0)
    return img_final



def main():
    #Criando janelas
    JANELA_FOREGROUND = "VIDEO ORIGINAL"
    JANELA_BACKGROUND = "IMG FUNDO"
    JANELA_RESULTADO = "IMG RESULT"
    cv2.namedWindow(JANELA_RESULTADO, cv2.WINDOW_GUI_NORMAL)
    cv2.namedWindow(JANELA_FOREGROUND, cv2.WINDOW_GUI_NORMAL)
    cv2.namedWindow(JANELA_BACKGROUND, cv2.WINDOW_GUI_NORMAL)
    cv2.resizeWindow(JANELA_FOREGROUND, 400,300)
    cv2.resizeWindow(JANELA_BACKGROUND, 400, 300)
    cv2.resizeWindow(JANELA_RESULTADO, 400, 300)

    #Abrindo recursos
    video = cv2.VideoCapture("recursos/videoplayback.mp4")
    background = cv2.imread("recursos/konoha.jpg", cv2.IMREAD_COLOR)

    cv2.imshow(JANELA_BACKGROUND, background)
    while video.isOpened():
        retorno, frame = video.read()
        if not retorno:
            print("Não foi possível exibir o vídeo")
            break


        #Exibindo video
        img_cortada = removerFundoVerde(frame, background)
        cv2.imshow(JANELA_FOREGROUND,frame)
        cv2.imshow(JANELA_RESULTADO, img_cortada)
        if cv2.waitKey(1) == ord('q'):
            break

    #Liberando memória
    video.release()
    cv2.destroyAllWindows()

main()