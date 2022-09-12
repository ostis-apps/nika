import * as React from 'react';

export const About = () => {
    return (
        <div className="about-page-container">
            <div className="about-page">
                <h1 className="about-page-text" style={{textAlign: "justify", marginLeft: "100px", marginRight: "100px"}}>
                    Обучающая диалоговая экспертная система <b>NIKA</b> (<b>N</b>ika is an <b>I</b>ntelligent <b>K</b>nowledge-driven <b>A</b>ssistant),
                    разработанный на основе технологии{' '}
                        <a href="http://ims.ostis.net/" className="text">
                          OSTIS
                        </a>
                    . Обучающая диалоговая экспертная ostis-система NIKA полностью заменяет участие Обучающей
                    диалоговой экспертной системы ЭКО в образовательном процессе по обучению студентов
                    специальностей Белорусского государственного университета информатики и радиоэлектроники
                    интеллектуальным экспертным системам. Новый вариант реализации системы такого класса позволяет
                    расширить круг решаемых задач, устраняет недостатки ранее использованного своего аналога и
                    упрощает и автоматизирует процесс обучения студентов специальностей Белорусского
                    государственного университета информатики и радиоэлектроники в контексте дисциплин, в которых
                    изучаются интеллектуальные экспертные системы.
                </h1>
                <h1 className="about-page-text">
                    Разработано{' '}
                        <a href="https://sem.systems/" className="text">
                          Intelligent Semantic Systems LLC
                        </a>
                    , Все права защищены.{' '}
                </h1>
            </div>
        </div>
    );
}
