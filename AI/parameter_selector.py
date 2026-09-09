import os
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import LeaveOneOut, cross_val_predict
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATASET = os.path.join(BASE_DIR, "day19_ai_dataset.csv")
RESULTS_DIR = os.path.join(BASE_DIR, "results")

os.makedirs(RESULTS_DIR, exist_ok=True)

# ------------------------------------------------------------
# Load dataset
# ------------------------------------------------------------
df = pd.read_csv(DATASET)

features = [
    "n",
    "h",
    "d",
    "b",
    "k",
    "w",
    "signature_bytes",
]

target = "sign_us"

X = df[features]
y = df[target]

# ------------------------------------------------------------
# Random Forest + Leave-One-Out Cross Validation
# ------------------------------------------------------------
model = RandomForestRegressor(
    n_estimators=300,
    random_state=42
)

loo = LeaveOneOut()

predictions = cross_val_predict(
    model,
    X,
    y,
    cv=loo,
    n_jobs=-1
)

mae = mean_absolute_error(y, predictions)
rmse = mean_squared_error(y, predictions) ** 0.5
r2 = r2_score(y, predictions)

# ------------------------------------------------------------
# Train final model
# ------------------------------------------------------------
model.fit(X, y)

importance = pd.DataFrame({
    "feature": features,
    "importance": model.feature_importances_
}).sort_values(
    "importance",
    ascending=False
)

# ------------------------------------------------------------
# Parameter ranking
# ------------------------------------------------------------
parameter_rows = []

for parameter, group in df.groupby("parameter", sort=False):

    representative = group.iloc[0]

    row = {
        "parameter": parameter,
        "n": representative["n"],
        "h": representative["h"],
        "d": representative["d"],
        "b": representative["b"],
        "k": representative["k"],
        "w": representative["w"],
        "signature_bytes": representative["signature_bytes"],
        "keygen_us": group["keygen_us"].mean(),
        "verify_us": group["verify_us"].mean(),
        "signature_bytes_mean": group["signature_bytes"].mean(),
        "public_key_bytes": representative["public_key_bytes"],
        "secret_key_bytes": representative["secret_key_bytes"],
    }

    feature_row = pd.DataFrame([[
        row["n"],
        row["h"],
        row["d"],
        row["b"],
        row["k"],
        row["w"],
        row["signature_bytes"],
    ]], columns=features)

    row["predicted_sign_us"] = model.predict(feature_row)[0]

    if "128" in parameter:
        row["security_bits"] = 128
    elif "192" in parameter:
        row["security_bits"] = 192
    elif "256" in parameter:
        row["security_bits"] = 256
    else:
        row["security_bits"] = 0

    parameter_rows.append(row)

ranking = pd.DataFrame(parameter_rows)

# ------------------------------------------------------------
# Multi-criteria normalization
# ------------------------------------------------------------
def minmax_low(series):
    if series.max() == series.min():
        return pd.Series([1.0] * len(series), index=series.index)
    return 1 - (
        (series - series.min()) /
        (series.max() - series.min())
    )

def minmax_high(series):
    if series.max() == series.min():
        return pd.Series([1.0] * len(series), index=series.index)
    return (
        (series - series.min()) /
        (series.max() - series.min())
    )

ranking["sign_score"] = minmax_low(
    ranking["predicted_sign_us"]
)

ranking["keygen_score"] = minmax_low(
    ranking["keygen_us"]
)

ranking["verify_score"] = minmax_low(
    ranking["verify_us"]
)

ranking["signature_score"] = minmax_low(
    ranking["signature_bytes_mean"]
)

ranking["security_score"] = minmax_high(
    ranking["security_bits"]
)

# Weights:
# 40% signing
# 15% key generation
# 10% verification
# 15% signature size
# 20% security
ranking["final_score"] = (
    0.40 * ranking["sign_score"] +
    0.15 * ranking["keygen_score"] +
    0.10 * ranking["verify_score"] +
    0.15 * ranking["signature_score"] +
    0.20 * ranking["security_score"]
)

ranking = ranking.sort_values(
    "final_score",
    ascending=False
).reset_index(drop=True)

ranking.insert(0, "rank", range(1, len(ranking) + 1))

recommendation = ranking.iloc[0]["parameter"]

# ------------------------------------------------------------
# Save prediction results
# ------------------------------------------------------------
prediction_df = df.copy()
prediction_df["predicted_sign_us"] = predictions
prediction_df["prediction_error_us"] = (
    prediction_df["predicted_sign_us"] -
    prediction_df["sign_us"]
)

prediction_df.to_csv(
    os.path.join(RESULTS_DIR, "ai_predictions.csv"),
    index=False
)

importance.to_csv(
    os.path.join(RESULTS_DIR, "ai_feature_importance.csv"),
    index=False
)

ranking.to_csv(
    os.path.join(RESULTS_DIR, "ai_parameter_ranking.csv"),
    index=False
)

# ------------------------------------------------------------
# Save text report
# ------------------------------------------------------------
report_path = os.path.join(
    RESULTS_DIR,
    "ai_parameter_selection_report.txt"
)

with open(report_path, "w") as f:

    f.write("===== AI-SUPPORTED SPHINCS+ PARAMETER SELECTION =====\n\n")

    f.write(f"Dataset observations : {len(df)}\n")
    f.write("Model                : RandomForestRegressor\n")
    f.write("Estimators            : 300\n")
    f.write("Random state          : 42\n")
    f.write("Cross-validation      : Leave-One-Out\n\n")

    f.write("MODEL PERFORMANCE\n")
    f.write(f"MAE  : {mae:.2f} us\n")
    f.write(f"RMSE : {rmse:.2f} us\n")
    f.write(f"R2   : {r2:.4f}\n\n")

    f.write("FEATURE IMPORTANCE\n")
    for _, row in importance.iterrows():
        f.write(
            f"{row['feature']:20s} "
            f"{row['importance']:.6f}\n"
        )

    f.write("\nPARAMETER RANKING\n")
    f.write(
        ranking[
            [
                "rank",
                "parameter",
                "security_bits",
                "predicted_sign_us",
                "keygen_us",
                "verify_us",
                "signature_bytes_mean",
                "final_score",
            ]
        ].to_string(index=False)
    )

    f.write("\n\nAI RECOMMENDATION\n")
    f.write(f"{recommendation}\n")

    f.write("\nSCORING WEIGHTS\n")
    f.write("Signing latency      : 0.40\n")
    f.write("Key generation       : 0.15\n")
    f.write("Verification         : 0.10\n")
    f.write("Signature size       : 0.15\n")
    f.write("Security level       : 0.20\n")

print("===== AI-SUPPORTED SPHINCS+ PARAMETER SELECTION =====")
print()
print(f"Dataset observations : {len(df)}")
print("Model                : RandomForestRegressor")
print("Estimators           : 300")
print("Cross-validation     : Leave-One-Out")
print()
print("MODEL PERFORMANCE")
print(f"MAE  : {mae:.2f} us")
print(f"RMSE : {rmse:.2f} us")
print(f"R2   : {r2:.4f}")
print()
print("FEATURE IMPORTANCE")
print(importance.to_string(index=False))
print()
print("PARAMETER RANKING")
print(
    ranking[
        [
            "rank",
            "parameter",
            "security_bits",
            "predicted_sign_us",
            "keygen_us",
            "verify_us",
            "signature_bytes_mean",
            "final_score",
        ]
    ].to_string(index=False)
)
print()
print(f"AI RECOMMENDATION : {recommendation}")
print()
print(f"Report saved to: {report_path}")
