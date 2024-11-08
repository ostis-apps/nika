"""
This code creates some test agent and registers until the user stops the process.
For this we wait for SIGINT.
"""
import gspread
import pandas as pd
import os
from google.oauth2 import service_account

import logging
from sc_client.models import ScAddr
from sc_client.constants import sc_types

from sc_client.client import template_search
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_kpm import ScAgentClassic, ScModule, ScResult, ScServer
from sc_kpm.utils import (
    create_link,
    get_link_content_data,
    check_edge, create_edge,
)
from sc_kpm.utils.action_utils import (
    finish_action_with_status,
    get_action_arguments
)
from sc_kpm import ScKeynodes

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)

class CourseAgent(ScAgentClassic):
    def __init__(self):
        super().__init__("action_show_course")

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:
        result = self.run(action_element)
        is_successful = result == ScResult.OK
        finish_action_with_status(action_element, is_successful)
        self.logger.info("CourseAgent finished %s",
                         "successfully" if is_successful else "unsuccessfully")
        return result

    def get_groups_sheets(self, spreadsheet):
        sheets = spreadsheet.worksheets()
        i = 0
        while i < len(sheets):
            if sheets[i].title.isdigit(): i+=1
            else: sheets.pop(i)
        return sheets

    def run(self, action_node: ScAddr) -> ScResult:
        self.logger.info("CourseAgent started")

        try:
            message_addr = get_action_arguments(action_node, 1)[0]
            message_type = ScKeynodes.resolve(
                "concept_message_about_course", sc_types.NODE_CONST_CLASS)

            if not check_edge(sc_types.EDGE_ACCESS_VAR_POS_PERM, message_type, message_addr):
                self.logger.info(
                    f"CourseAgent: the message isn’t about course")
                return ScResult.OK
        except:
            self.logger.info(f"CourseAgent: finished with an error")
            return ScResult.ERROR

        course_work = 'course_work_12170x'
        projects = set()

        path_to_directory = os.getcwd()

        SERVICE_ACCOUNT_FILE = f'{path_to_directory}/modules/messageProcessingModule/keys.json'.replace('/scripts', '/problem-solver/py')
        #SERVICE_ACCOUNT_FILE = '/home/anna/projects/forks/nika/problem-solver/py/modules/messageProcessingModule/keys.json'
        SCOPES = ["https://www.googleapis.com/auth/spreadsheets.readonly"]

        credentials = service_account.Credentials.from_service_account_file(
            SERVICE_ACCOUNT_FILE, scopes=SCOPES)
        client = gspread.authorize(credentials)

        url = "https://docs.google.com/spreadsheets/d/1KrXHlI6p_qM77Ps0P8WgbP3Tusjru40g1yNFBv9EE08/edit#gid=0"
        spreadsheet = client.open_by_url(url)
        sheets = self.get_groups_sheets(spreadsheet)

        groups_name = f'{sheets[0].title[:-2]}xx'
        path_to_directory = path_to_directory.replace('scripts', 'kb/course_works')

        file = open(f'{path_to_directory}/{groups_name}.scs', 'a+', encoding = 'utf-8')

        file.write(f"{course_work}\n<-sc_node_not_relation;\n<-concept_course_work;;\n\n")

        for sheet in sheets:
            students = pd.DataFrame(data=sheet.get_values())
            for i in range(len(students)):

                fullname = students[1][i]
                if fullname == 'ФИО' or fullname =='': continue
                file.write(f"...\n<-sc_node_not_relation;\n<-concept_student;\n=>nrel__main_idtf:[{fullname}](*<-lang_ru;;*);\n")

                grade = students[24][i]
                if int(grade) > 0:
                    file.write(f"=>nrel_final_grade:{grade}(*\n\t<-sc_node_not_relation;;\n\t<-concept_number;;*);;\n")

                current_project = students[2][i]
                file.write(f"=>nrel_project:...(*\n\t<-sc_node_not_relation;;\n\t<-concept_project;;\n\t=>nrel__main_idtf:[{current_project}](*<-lang_ru;;*);;\n")

                if not(current_project in projects):
                    curator = students[4][i]
                    inspector = students[5][i]
                    file.write(f"\t=>nrel_curator:...(*\n\t\t<-sc_node_not_relation;;\n\t\t<-concept_teacher;;\n\t\t=>nrel__main_idtf:[{curator}](*<-lang_ru;;*);;*);;\n")
                    file.write(f"\t=>nrel_inspector:...(*\n\t\t<-sc_node_not_relation;;\n\t\t<-concept_teacher;;\n\t\t=>nrel__main_idtf:[{inspector}](*<-lang_ru;;*);;*);;\n")
                    file.write(f"\t<=nrel_project:{course_work};;")
                    best_project = students[21][i]
                    if best_project != 'FALSE':
                        file.write(f"\t<=nrel_best_project:{course_work};;")
                    projects.add(current_project)

                file.write("*);;\n\n")
                due_date = students[6][i]

        file.write(f"{course_work}\n=>nrel_assignment_sheet_due_date:{due_date}(*\n\t<-sc_node_not_relation;;\n\t<-concept_date;;*);;")
        file.close()

        return ScResult.OK
