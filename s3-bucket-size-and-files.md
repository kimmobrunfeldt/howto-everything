# Get S3 bucket total size and file count

Save the following script and run `bash get-s3-info.sh <bucketname>`.

**get-s3-info.sh**

```bash
#!/bin/bash
bucket=$1
now=$(date +%s)

if [[ -z "${AWS_ACCESS_KEY_ID}" ]]; then
  echo "Missing environment variable: AWS_ACCESS_KEY_ID"
  exit 1;
elif [[ -z "${AWS_SECRET_ACCESS_KEY}" ]]; then
  echo "Missing environment variable: AWS_SECRET_ACCESS_KEY"
  exit 1;
fi

echo -e "\n\nTotal size of S3 bucket named: $bucket"
aws cloudwatch get-metric-statistics --namespace AWS/S3 --start-time "$(echo "$now - 86400" | bc)" --end-time "$now" --period 86400 --statistics Average --region eu-west-1 --metric-name BucketSizeBytes --dimensions Name=BucketName,Value="$bucket" Name=StorageType,Value=StandardStorage

echo -e "\n\nTotal amount of files in S3 bucket named: $bucket"
aws cloudwatch get-metric-statistics --namespace AWS/S3 --start-time "$(echo "$now - 86400" | bc)" --end-time "$now" --period 86400 --statistics Average --region eu-west-1 --metric-name NumberOfObjects --dimensions Name=BucketName,Value="$bucket" Name=StorageType,Value=AllStorageTypes

```bash
