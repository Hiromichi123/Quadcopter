from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'vision_py'

setup(
    name=package_name,
    version='0.0.0',
    # scripts/ is a Python package in this workspace and provides ROS2 entry points.
    packages=find_packages(include=['scripts', 'scripts.*']),

    install_requires=[
        'setuptools',
        'rclpy',
    ],

    zip_safe=True,
    maintainer='Hiromichi123',
    maintainer_email='2271612727@qq.com',
    description='vision package',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'vision_node = scripts.vision_node:main',
            'dvs_raw_serial_node = scripts.dvs_raw_serial_node:main',
            'dvs_minimal_record_node = scripts.dvs_minimal_record_node:main',
        ],
    },
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        (os.path.join("share", package_name), ["package.xml"]),
        (os.path.join("share", package_name, "scripts"), glob("scripts/*.py")),
    ],
)